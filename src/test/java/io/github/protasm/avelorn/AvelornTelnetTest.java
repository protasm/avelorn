package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.communication.protocol.UTF8Frames;
import io.github.protasm.jvmud.communication.transport.telnet.TelnetServer;
import io.github.protasm.jvmud.engine.instance.MudInstance;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

/** Player-visible Avelorn coverage over JVMud's Telnet transport. */
class AvelornTelnetTest {
    @TempDir
    Path tempDir;

    @Test
    void accountAndGameplayOutputUsesTelnetLineEndings() throws Exception {
        Path avelorn = AvelornFixture.copyTo(tempDir.resolve("avelorn"));

        MudInstance world = MudInstance.boot(avelorn, AvelornFixture.CONFIG_PATH);
        String relaySecret = "avelorn-worker-fixture";
        try (TelnetServer worker = new TelnetServer(world, relaySecret)) {
            world.startExecution();
            worker.start();

            try (Socket socket = connect(worker, relaySecret)) {
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
                        socket, "Gender (male/female): "));
                writeLine(socket, "female");
                assertTelnetText(readUntilQuietAfterContains(
                        socket, "Class (fighter/ranger/mage/cleric): "));
                writeLine(socket, "mage");

                String firstLook = readUntilQuietAfterContains(socket, "Exits:");
                assertTrue(firstLook.contains("Westwatch Guest-House"), printable(firstLook));
                assertNoTerrainStats(firstLook);
                assertTrue(firstLook.contains("find the door.\r\n\r\nThe landing"), printable(firstLook));

                writeLine(socket, "examine iona");
                String iona = readUntilQuietAfterContains(socket, "\r\n> ");
                assertTrue(iona.contains("Iona Venn is the Westwatch guest-house keeper.\r\n"
                        + "Her second register lists guests who left without naming a destination.\r\n"
                        + "Type work to accept the assignment or report when it is complete.\r\n"), printable(iona));
                assertTelnetText(iona);

                writeLine(socket, "south");
                String landing = readUntilQuietAfterContains(socket, "Exits:");
                assertTrue(landing.contains("Westwatch Landing"), printable(landing));
                assertTelnetText(landing);

                writeLine(socket, "out");
                String dunes = readUntilQuietAfterContains(socket, "Exits:");
                assertTrue(dunes.contains("Westwatch Dunes"), printable(dunes));
                assertTrue(dunes.contains("Wind-rippled dunes rise behind the sandy shore."), printable(dunes));
                assertNoTerrainStats(dunes);

                writeLine(socket, "north");
                String north = readUntilQuietAfterContains(socket, "Exits:");
                assertTrue(north.contains("The Wild"), printable(north));
                assertNoTerrainStats(north);

                writeLine(socket, "quit");
                String farewell = readUntilSocketClosed(socket);
                assertTrue(farewell.contains("Farewell."), printable(farewell));
                assertTelnetText(farewell);
            }

            try (Socket socket = connect(worker, relaySecret)) {
                socket.setSoTimeout(5000);
                assertTelnetText(readUntilQuietAfterContains(socket, "Account ID: "));
                writeLine(socket, "line_endings");
                assertTelnetText(readUntilQuietAfterContains(socket, "Password: "));

                writeLine(socket, "Wrong1!");
                String rejected = readUntilQuietAfterContains(socket, "Password: ");
                assertTrue(rejected.contains("That password did not match."), printable(rejected));
                assertTelnetText(rejected);

                writeLine(socket, "Avelorn1!");
                String returningLook = readUntilQuietAfterContains(socket, "Exits:");
                assertTrue(returningLook.contains("Welcome back, Mira valewood."), printable(returningLook));
                assertTrue(returningLook.contains("The Wild"), printable(returningLook));
                assertTelnetText(returningLook);

                writeLine(socket, "quit");
                assertTelnetText(readUntilSocketClosed(socket));
            }
        } finally {
            world.shutdown("Test finished.");
        }
    }

    @Test
    void bigDirectionPasteLeavesAnotherTelnetPlayerResponsive() throws Exception {
        Path avelorn = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        MudInstance world = MudInstance.boot(avelorn, AvelornFixture.CONFIG_PATH);
        String relaySecret = "avelorn-paste-fixture";
        try (TelnetServer worker = new TelnetServer(world, relaySecret)) {
            world.startExecution();
            worker.start();
            try (Socket traveler = connect(worker, relaySecret);
                    Socket observer = connect(worker, relaySecret)) {
                createRanger(traveler, "paste_traveler", "Arden Vale");
                createRanger(observer, "paste_observer", "Celyn Ward");
                writeLine(traveler, "south");
                readUntilQuietAfterContains(traveler, "Westwatch Landing");
                writeLine(traveler, "out");
                readUntilQuietAfterContains(traveler, "Westwatch Dunes");

                traveler.getOutputStream().write("east\n".repeat(10_000).getBytes(StandardCharsets.UTF_8));
                traveler.getOutputStream().flush();
                writeLine(observer, "who");
                String who = readUntilQuietAfterContains(observer, "Celyn ward");
                assertTrue(who.contains("Adventurers (2 online)"), printable(who));

                String overflow = readUntilQuietAfterContains(traveler, "Command queue full.");
                assertTrue(overflow.contains("Command queue full."), printable(overflow));
                writeLine(traveler, "stop");
                assertTrue(readUntilQuietAfterContains(traveler, "You stop and clear")
                        .contains("You stop and clear"));
                writeLine(traveler, "queue");
                String queues = readUntilQuietAfterContains(traveler, "Movement queue: 0/8.");
                assertTrue(queues.contains("Command queue: 0/16."), printable(queues));
            }
        } finally {
            world.shutdown("Test finished.");
        }
    }

    private void createRanger(Socket socket, String account, String name) throws Exception {
        socket.setSoTimeout(10_000);
        readUntilQuietAfterContains(socket, "Account ID: ");
        writeLine(socket, account);
        readUntilQuietAfterContains(socket, "Create it? (yes/no) ");
        writeLine(socket, "yes");
        readUntilQuietAfterContains(socket, "Choose a password: ");
        writeLine(socket, "Avelorn1!");
        readUntilQuietAfterContains(socket, "Password again: ");
        writeLine(socket, "Avelorn1!");
        readUntilQuietAfterContains(socket, "Character name: ");
        writeLine(socket, name);
        readUntilQuietAfterContains(socket, "Gender (male/female): ");
        writeLine(socket, "female");
        readUntilQuietAfterContains(socket, "Class (fighter/ranger/mage/cleric): ");
        writeLine(socket, "ranger");
        String before = readUntilQuietAfterContains(socket, "Westwatch Guest-House");
        assertTrue(before.contains("Welcome to Avelorn, "), printable(before));
    }

    private Socket connect(TelnetServer worker, String relaySecret) throws IOException {
        Socket socket = new Socket("127.0.0.1", worker.port());
        try {
            // A hosted worker receives these frames from JVMud's gateway before Telnet input.
            var output = new DataOutputStream(socket.getOutputStream());
            UTF8Frames.write(output, relaySecret, 256);
            UTF8Frames.write(output, "127.0.0.1", 256);
            output.flush();
            return socket;
        } catch (IOException e) {
            socket.close();
            throw e;
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

    private void assertNoTerrainStats(String text) {
        assertFalse(text.contains("Terrain:"), printable(text));
        assertFalse(text.contains("Elevation:"), printable(text));
        assertTelnetText(text);
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
