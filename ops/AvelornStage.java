import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.KeyStore;
import java.security.cert.CertificateFactory;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.LongAccumulator;
import java.util.concurrent.atomic.LongAdder;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;

/** Local TLS access and a headless test driver for a private Avelorn staging service. */
public final class AvelornStage {
    private static final String HOST = "127.0.0.1";
    private static final String GUEST_HOUSE = "Westwatch Guest-House";
    private static final String LANDING = "Westwatch Landing";
    private static final String DUNES = "Westwatch Dunes";
    private static final String WILDERNESS = "The Wild";
    private static final int CLIENT_TIMEOUT = 20_000;

    public static void main(String[] args) throws Exception {
        if (args.length == 5 && args[0].equals("proxy")) {
            proxy(port(args[1]), port(args[2]), Path.of(args[3]), Path.of(args[4]));
        } else if (args.length == 6 && args[0].equals("soak")) {
            int seconds = Integer.parseInt(args[4]);
            int users = Integer.parseInt(args[5]);
            if (seconds < 1 || seconds > 86_400 || users < 1 || users > 32) {
                throw new IllegalArgumentException("Use 1–86400 seconds and 1–32 clients.");
            }
            soak(port(args[1]), Path.of(args[2]), Path.of(args[3]), seconds, users);
        } else {
            throw new IllegalArgumentException("Use the packaged avelorn-stage launcher.");
        }
    }

    private static int port(String value) {
        int result = Integer.parseInt(value);
        if (result < 1024 || result > 65_535) {
            throw new IllegalArgumentException("Use an unprivileged local port.");
        }
        return result;
    }

    private static void proxy(int tlsPort, int backendPort, Path storePath, Path passwordFile) throws Exception {
        SSLContext context = serverContext(storePath, passwordFile);
        InetAddress address = InetAddress.getByName(HOST);
        try (var listener = (SSLServerSocket) context.getServerSocketFactory().createServerSocket();
                var sessions = Executors.newVirtualThreadPerTaskExecutor()) {
            listener.setReuseAddress(true);
            listener.bind(new InetSocketAddress(address, tlsPort));
            listener.setEnabledProtocols(new String[]{"TLSv1.3", "TLSv1.2"});
            System.out.println("Listening on " + HOST + ":" + tlsPort);
            while (true) {
                var client = (SSLSocket) listener.accept();
                sessions.execute(() -> {
                    try {
                        client.setSoTimeout(10_000);
                        client.startHandshake();
                        client.setSoTimeout(0);
                        var backend = new Socket();
                        try {
                            backend.connect(new InetSocketAddress(address, backendPort), 5_000);
                            sessions.execute(() -> relay(backend, client));
                            relay(client, backend);
                        } finally {
                            close(backend);
                        }
                    } catch (IOException ignored) {
                        // A client can disconnect during the handshake or while the worker restarts.
                    } finally {
                        close(client);
                    }
                });
            }
        }
    }

    private static void relay(Socket from, Socket to) {
        try {
            from.getInputStream().transferTo(to.getOutputStream());
            to.getOutputStream().flush();
        } catch (IOException ignored) {
            // Either peer can leave first.
        } finally {
            close(from);
            close(to);
        }
    }

    private static void close(Socket socket) {
        try {
            socket.close();
        } catch (IOException ignored) {
            // The other relay direction may have closed the socket already.
        }
    }

    private static SSLContext serverContext(Path storePath, Path passwordFile) throws Exception {
        char[] password = Files.readString(passwordFile).strip().toCharArray();
        var keys = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
        try (var input = Files.newInputStream(storePath)) {
            var store = KeyStore.getInstance("PKCS12");
            store.load(input, password);
            keys.init(store, password);
        } finally {
            Arrays.fill(password, '\0');
        }
        var context = SSLContext.getInstance("TLS");
        context.init(keys.getKeyManagers(), null, null);
        return context;
    }

    private static SSLContext clientContext(Path certificateFile) throws Exception {
        var trust = KeyStore.getInstance("PKCS12");
        trust.load(null, null);
        try (var input = Files.newInputStream(certificateFile)) {
            var factory = CertificateFactory.getInstance("X.509");
            trust.setCertificateEntry("avelorn-stage", factory.generateCertificate(input));
        }
        var managers = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        managers.init(trust);
        var context = SSLContext.getInstance("TLS");
        context.init(null, managers.getTrustManagers(), null);
        return context;
    }

    private static void soak(int port, Path certificate, Path passwordFile, int seconds, int users) throws Exception {
        SSLContext context = clientContext(certificate);
        String password = Files.readString(passwordFile).strip();
        long started = System.nanoTime();
        long deadline = started + TimeUnit.SECONDS.toNanos(seconds);
        var progress = new Progress();
        System.out.printf("%s soak_start seconds=%d clients=%d endpoint=%s:%d%n",
                Instant.now(), seconds, users, HOST, port);
        try (var clients = Executors.newVirtualThreadPerTaskExecutor()) {
            List<Future<Boolean>> results = new ArrayList<>();
            for (int index = 0; index < users; index++) {
                int number = index;
                results.add(clients.submit(() -> play(context, port, password, number, deadline, progress)));
            }
            while (remainingMillis(deadline) > 0) {
                Thread.sleep(Math.min(30_000, remainingMillis(deadline)));
                progress.report("soak_progress", started);
            }
            boolean allPlayed = true;
            for (Future<Boolean> result : results) {
                allPlayed &= result.get();
            }
            progress.report("soak_end", started);
            if (!allPlayed || progress.peak.get() < users || progress.failures.sum() > 0) {
                throw new IOException("Staging soak encountered connection or gameplay failures.");
            }
        }
    }

    private static boolean play(SSLContext context, int port, String password, int number, long deadline,
            Progress progress) throws InterruptedException {
        boolean played = false;
        int round = number;
        while (remainingMillis(deadline) > 0) {
            boolean loggedIn = false;
            try (var client = new Client(context, port)) {
                Login login = client.signIn("stage_" + number, password, number);
                progress.active.increment();
                progress.peak.accumulate(progress.active.sum());
                progress.logins.increment();
                if (login.returning()) { progress.returning.increment(); }
                loggedIn = true;
                returnHome(client, login.room(), progress);
                while (remainingMillis(deadline) > 0) {
                    switch (round % 6) {
                        case 0 -> client.playCommand("look", "Exits:", progress);
                        case 1 -> client.playCommand("score", "Health ", progress);
                        case 2 -> client.playCommand("inventory", "Carried weight:", progress);
                        case 3 -> client.playCommand("who", "Adventurers (", progress);
                        case 4 -> client.playCommand("save", "Your character has been saved.", progress);
                        case 5 -> exploreDunes(client, progress);
                        default -> throw new IllegalStateException();
                    }
                    played = true;
                    round += 1;
                    long pause = Math.min(2_000 + number % 9 * 137, remainingMillis(deadline));
                    if (pause > 0) { Thread.sleep(pause); }
                }
            } catch (IOException failure) {
                progress.failures.increment();
                System.err.printf("%s client=%d error=%s%n", Instant.now(), number, failure.getMessage());
                long pause = Math.min(1_500, remainingMillis(deadline));
                if (pause > 0) { Thread.sleep(pause); }
            } finally {
                if (loggedIn) { progress.active.decrement(); }
            }
        }
        return played;
    }

    private static void exploreDunes(Client client, Progress progress) throws IOException {
        int before = client.travelReserve(progress);
        client.walk("south", LANDING, progress);
        client.walk("out", DUNES, progress);
        client.walk("north", WILDERNESS, progress);
        int spent = client.travelReserve(progress);
        if (spent >= before) {
            throw new IOException("Wilderness movement did not spend travel reserve.");
        }
        returnHome(client, WILDERNESS, progress);
        int home = client.travelReserve(progress);
        client.playCommand("rest", "You rest and recover", progress);
        int restored = client.travelReserve(progress);
        if (home > spent || restored != 100 || restored <= home) {
            throw new IOException("An inn rest failed to restore spent travel reserve.");
        }
        progress.excursions.increment();
        progress.reserveSpent.add(before - home);
        progress.lowestReserve.accumulate(home);
    }

    private static void returnHome(Client client, String room, Progress progress) throws IOException {
        if (room.equals(WILDERNESS)) {
            client.walk("south", DUNES, progress);
            room = DUNES;
        }
        if (room.equals(DUNES)) {
            client.walk("enter", LANDING, progress);
            room = LANDING;
        }
        if (room.equals(LANDING)) {
            client.walk("north", GUEST_HOUSE, progress);
        }
    }

    private static long remainingMillis(long deadline) {
        return Math.max(0, TimeUnit.NANOSECONDS.toMillis(deadline - System.nanoTime()));
    }

    private static final class Progress {
        private final LongAdder active = new LongAdder();
        private final LongAdder logins = new LongAdder();
        private final LongAdder returning = new LongAdder();
        private final LongAdder commands = new LongAdder();
        private final LongAdder failures = new LongAdder();
        private final LongAdder excursions = new LongAdder();
        private final LongAdder reserveSpent = new LongAdder();
        private final LongAccumulator lowestReserve = new LongAccumulator(Long::min, 100);
        private final LongAccumulator peak = new LongAccumulator(Long::max, 0);
        private final LongAccumulator longest = new LongAccumulator(Long::max, 0);

        private void commandCompleted(long started) {
            commands.increment();
            longest.accumulate(TimeUnit.NANOSECONDS.toMillis(System.nanoTime() - started));
        }

        private void report(String event, long started) {
            System.out.printf(
                    "%s %s elapsed_s=%d active=%d peak_active=%d logins=%d returning=%d commands=%d failures=%d "
                            + "max_command_ms=%d excursions=%d reserve_spent=%d lowest_reserve=%d%n",
                    Instant.now(), event, TimeUnit.NANOSECONDS.toSeconds(System.nanoTime() - started),
                    active.sum(), peak.get(), logins.sum(), returning.sum(), commands.sum(), failures.sum(),
                    longest.get(), excursions.sum(), reserveSpent.sum(), lowestReserve.get());
        }
    }

    private record Login(String room, boolean returning) {}

    private static final class Client implements AutoCloseable {
        private final SSLSocket socket;
        private final InputStream input;

        private Client(SSLContext context, int port) throws IOException {
            socket = (SSLSocket) context.getSocketFactory().createSocket();
            try {
                socket.connect(new InetSocketAddress(HOST, port), 5_000);
                socket.setSoTimeout(CLIENT_TIMEOUT);
                var parameters = socket.getSSLParameters();
                parameters.setEndpointIdentificationAlgorithm("HTTPS");
                socket.setSSLParameters(parameters);
                socket.startHandshake();
                input = new BufferedInputStream(socket.getInputStream());
            } catch (IOException failure) {
                AvelornStage.close(socket);
                throw failure;
            }
        }

        private Login signIn(String account, String password, int number) throws IOException {
            await("Account ID: ");
            send(account);
            boolean returning = await("Create it? (yes/no) ", "Password: ").startsWith("Password");
            if (!returning) {
                command("yes", "Choose a password: ");
                command(password, "Password again: ");
                command(password, "Character name: ");
                char initial = (char) ('A' + number / 26);
                char suffix = (char) ('a' + number % 26);
                command("Stage " + initial + suffix, "Gender (male/female): ");
                command("female", "Class (fighter/ranger/mage/cleric): ");
                send("ranger");
                await("Welcome to Avelorn, ");
            } else {
                command(password, "Welcome back, ");
            }
            String room = await(GUEST_HOUSE, LANDING, DUNES, WILDERNESS);
            await("Exits:");
            return new Login(room, returning);
        }

        private void command(String line, String expected) throws IOException {
            send(line);
            await(expected);
        }

        private void playCommand(String line, String expected, Progress progress) throws IOException {
            long started = System.nanoTime();
            command(line, expected);
            progress.commandCompleted(started);
        }

        private void walk(String direction, String room, Progress progress) throws IOException {
            long started = System.nanoTime();
            command(direction, room);
            await("Exits:");
            progress.commandCompleted(started);
        }

        private int travelReserve(Progress progress) throws IOException {
            long started = System.nanoTime();
            command("queue", "Travel reserve: ");
            int reserve = 0;
            int digits = 0;
            for (int value = readText(); value != '/'; value = readText()) {
                if (value < '0' || value > '9' || ++digits > 3) {
                    throw new IOException("Invalid travel reserve in queue output.");
                }
                reserve = reserve * 10 + value - '0';
            }
            if (digits == 0 || reserve > 100) {
                throw new IOException("Invalid travel reserve in queue output.");
            }
            progress.commandCompleted(started);
            return reserve;
        }

        private void send(String line) throws IOException {
            var output = socket.getOutputStream();
            output.write((line + "\r\n").getBytes(StandardCharsets.UTF_8));
            output.flush();
        }

        private String await(String... markers) throws IOException {
            var buffer = new StringBuilder();
            try {
                for (int value = readText(); value >= 0; value = readText()) {
                    buffer.append((char) value);
                    for (String marker : markers) {
                        int start = buffer.length() - marker.length();
                        if (start >= 0 && buffer.substring(start).equals(marker)) {
                            return marker;
                        }
                    }
                    if (buffer.length() >= 65_536) { break; }
                }
            } catch (SocketTimeoutException failure) {
                throw new IOException("Timed out waiting for " + String.join(" or ", markers), failure);
            }
            throw new IOException("No expected game output for " + String.join(" or ", markers));
        }

        private int readText() throws IOException {
            while (true) {
                int value = input.read();
                if (value != 255) { return value; }
                int command = input.read();
                if (command == -1 || command == 255) { return command; }
                if (command >= 251 && command <= 254) { input.read(); }
                if (command == 250) {
                    int prior = 0;
                    while ((value = input.read()) >= 0 && (prior != 255 || value != 240)) {
                        prior = value;
                    }
                    if (value < 0) { return value; }
                }
            }
        }

        @Override
        public void close() {
            AvelornStage.close(socket);
        }
    }
}
