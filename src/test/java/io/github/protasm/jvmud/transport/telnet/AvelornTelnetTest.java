package io.github.protasm.jvmud.transport.telnet;

import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

/** Player-visible Avelorn coverage over JVMud's Telnet transport. */
class AvelornTelnetTest {
    @TempDir
    Path tempDir;

    @Test
    void accountAndGameplayOutputUsesTelnetLineEndings() throws Exception {
        Path avelorn = copyAvelorn();

        try (TelnetServer server =
                new TelnetServer("127.0.0.1", 0, avelorn, "jvmud/avelorn.config")) {
            server.start();
            String ruler = "+=========".repeat(8);

            try (Socket socket = new Socket("127.0.0.1", server.port())) {
                socket.setSoTimeout(5000);
                assertTelnetText(readUntilQuietAfterContains(socket, "Account ID: "));

                writeLine(socket, "line_endings");
                assertTelnetText(readUntilQuietAfterContains(socket, "Create it? (yes/no) "));
                writeLine(socket, "yes");
                assertTelnetText(readUntilQuietAfterContains(socket, "Choose a password: "));

                writeLine(socket, "short");
                String rejected = readUntilQuietAfterContains(socket, "Choose a password: ");
                assertTrue(rejected.startsWith("\r\n"), printable(rejected));
                assertTrue(rejected.contains("at least 8 characters"), printable(rejected));
                assertTelnetText(rejected);

                writeLine(socket, "Avelorn1!");
                assertTelnetText(readUntilQuietAfterContains(socket, "Password again: "));
                writeLine(socket, "Avelorn1!");
                assertTelnetText(readUntilQuietAfterContains(socket, "Character name: "));
                writeLine(socket, "Mira Valewood");
                assertTelnetText(readUntilQuietAfterContains(
                        socket, "Gender (male/female/non-binary): "));
                writeLine(socket, "non-binary");
                assertTelnetText(readUntilQuietAfterContains(
                        socket, "Class (fighter/ranger/mage/cleric): "));
                writeLine(socket, "mage");

                String firstLook = readUntilQuietAfterContains(socket, ruler);
                assertTrue(firstLook.contains("The Wilderness"), printable(firstLook));
                assertTrue(firstLook.contains("Terrain: Forested Foothill"), printable(firstLook));
                assertTelnetText(firstLook);

                writeLine(socket, "north");
                String north = readUntilQuietAfterContains(socket, ruler);
                assertTrue(north.contains("The Wilderness"), printable(north));
                assertTrue(north.contains("Terrain: Forested Foothill"), printable(north));
                assertTrue(north.contains("Elevation: 296 m"), printable(north));
                assertTelnetText(north);

                writeLine(socket, "quit");
                String farewell = readUntilSocketClosed(socket);
                assertTrue(farewell.contains("Farewell."), printable(farewell));
                assertTelnetText(farewell);
            }

            try (Socket socket = new Socket("127.0.0.1", server.port())) {
                socket.setSoTimeout(5000);
                assertTelnetText(readUntilQuietAfterContains(socket, "Account ID: "));
                writeLine(socket, "line_endings");
                assertTelnetText(readUntilQuietAfterContains(socket, "Password: "));

                writeLine(socket, "Wrong1!");
                String rejected = readUntilQuietAfterContains(socket, "Password: ");
                assertTrue(rejected.contains("That password did not match."), printable(rejected));
                assertTelnetText(rejected);

                writeLine(socket, "Avelorn1!");
                String returningLook = readUntilQuietAfterContains(socket, ruler);
                assertTrue(returningLook.contains("Welcome back, Mira valewood."), printable(returningLook));
                assertTrue(returningLook.contains("The Wilderness"), printable(returningLook));
                assertTelnetText(returningLook);

                writeLine(socket, "quit");
                assertTelnetText(readUntilSocketClosed(socket));
            }
        }
    }

    private Path copyAvelorn() throws IOException {
        Path source = Path.of(".").toAbsolutePath().normalize();
        Path target = tempDir.resolve("avelorn");
        try (var paths = Files.walk(source)) {
            paths.filter(path -> !excluded(source, path)).forEach(path -> copyPath(source, target, path));
        }
        return target;
    }

    private boolean excluded(Path source, Path path) {
        Path relative = source.relativize(path);
        return relative.startsWith(".git")
                || relative.startsWith("target")
                || (relative.startsWith("accounts") && path.getFileName().toString().endsWith(".o"));
    }

    private void copyPath(Path source, Path target, Path path) {
        try {
            Path destination = target.resolve(source.relativize(path));
            if (Files.isDirectory(path)) {
                Files.createDirectories(destination);
            } else {
                Files.copy(path, destination);
            }
        } catch (IOException e) {
            throw new IllegalStateException("Could not copy Avelorn fixture", e);
        }
    }

    private void writeLine(Socket socket, String line) throws IOException {
        socket.getOutputStream().write((line + "\n").getBytes(StandardCharsets.UTF_8));
        socket.getOutputStream().flush();
    }

    private String readUntilQuietAfterContains(Socket socket, String expected) throws Exception {
        StringBuilder output = new StringBuilder();
        while (!output.toString().contains(expected)) {
            int value = socket.getInputStream().read();
            if (value == -1) {
                return output.toString();
            }
            output.append((char) value);
        }
        long deadline = System.nanoTime() + 50_000_000L;
        while (System.nanoTime() < deadline) {
            while (socket.getInputStream().available() > 0) {
                int value = socket.getInputStream().read();
                if (value == -1) {
                    return output.toString();
                }
                output.append((char) value);
            }
            Thread.sleep(5);
        }
        return output.toString();
    }

    private String readUntilSocketClosed(Socket socket) throws IOException {
        StringBuilder output = new StringBuilder();
        int value;
        while ((value = socket.getInputStream().read()) != -1) {
            output.append((char) value);
        }
        return output.toString();
    }

    private void assertTelnetText(String text) {
        for (int index = 0; index < text.length(); index++) {
            if (text.charAt(index) == '\n') {
                assertTrue(index > 0 && text.charAt(index - 1) == '\r', printable(text));
            }
        }
    }

    private String printable(String text) {
        return text.replace("\r", "\\r").replace("\n", "\\n");
    }
}
