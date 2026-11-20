package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
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
    private static final int DEFAULT_WIDTH = 60;

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
                assertTrue(firstLook.contains("Westwatch Guest-House\r\n" + "+=========".repeat(6) + "\r\n"),
                        printable(firstLook));
                assertTrue(firstLook.matches("(?s).*find\\s+the\\s+door\\.\r\n\r\nThe landing.*"),
                        printable(firstLook));

                writeLine(socket, "examine iona");
                String iona = readUntilQuietAfterContains(socket, "\r\n> ");
                assertWrappedTelnetText(iona, DEFAULT_WIDTH);
                assertFalse(iona.contains("\r\n\r\n"), printable(iona));
                assertTrue(iona.endsWith("\r\n> "), printable(iona));
                String prose = iona.substring(0, iona.length() - 2);
                assertEquals("Iona Venn is the Westwatch guest-house keeper. "
                                + "She has posted a request for assistance. "
                                + "Iona keeps a second register of guests who leave without saying where they are going. "
                                + "Type work to accept the assignment or report when it is complete.",
                        prose.replaceAll("\\s+", " ").strip(), printable(iona));

                writeLine(socket, "inventory");
                String inventory = readUntilQuietAfterContains(socket, "\r\n> ");
                assertTrue(inventory.contains("\r\n  oak focus staff (equipped)\r\n"), printable(inventory));
                assertWrappedTelnetText(inventory, DEFAULT_WIDTH);

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

    @Test
    void speechWrapsAtTheLastFittingSpaceAndHyphenatesAnUnbrokenWord() throws Exception {
        Path avelorn = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        MudInstance world = MudInstance.boot(avelorn, AvelornFixture.CONFIG_PATH);
        String relaySecret = "avelorn-wrap-fixture";
        try (TelnetServer worker = new TelnetServer(world, relaySecret)) {
            world.startExecution();
            worker.start();
            try (Socket speaker = connect(worker, relaySecret);
                    Socket listener = connect(worker, relaySecret)) {
                createRanger(speaker, "wrap_speaker", "Arden Vale");
                createRanger(listener, "wrap_listener", "Celyn Ward");

                String fitting = "a".repeat(45) + " bbbb";
                writeLine(speaker, "say " + fitting + " tail");
                String greedy = readUntilQuietAfterContains(speaker, "\r\n> ");
                assertTrue(greedy.contains("You say, \"" + fitting + "\r\ntail\"\r\n"), printable(greedy));
                assertWrappedTelnetText(greedy, DEFAULT_WIDTH);
                assertWrappedTelnetText(readUntilQuietAfterContains(listener, "tail\"\r\n"), DEFAULT_WIDTH);

                String hyphenated = "x".repeat(59) + "-\r\nxx\"\r\n";
                writeLine(speaker, "say ready " + "x".repeat(61));
                String self = readUntilQuietAfterContains(speaker, "\r\n> ");
                String broadcast = readUntilQuietAfterContains(listener, "xx\"\r\n");
                assertTrue(self.contains("You say, \"ready\r\n" + hyphenated), printable(self));
                assertTrue(broadcast.contains("Arden vale says, \"ready\r\n" + hyphenated), printable(broadcast));
                assertWrappedTelnetText(self, DEFAULT_WIDTH);
                assertWrappedTelnetText(broadcast, DEFAULT_WIDTH);
            }
        } finally {
            world.shutdown("Test finished.");
        }
    }

    @Test
    void displayWidthAppliesToEachPlayerAndPersistsAcrossLogin() throws Exception {
        Path avelorn = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        MudInstance world = MudInstance.boot(avelorn, AvelornFixture.CONFIG_PATH);
        String relaySecret = "avelorn-width-fixture";
        int narrowWidth = 37;
        int wideWidth = 120;
        String narrowRuler = "Westwatch Guest-House\r\n" + "+=========".repeat(3) + "+======\r\n";
        try (TelnetServer worker = new TelnetServer(world, relaySecret)) {
            world.startExecution();
            worker.start();
            try (Socket speaker = connect(worker, relaySecret);
                    Socket listener = connect(worker, relaySecret)) {
                createRanger(speaker, "width_speaker", "Arden Vale");
                createRanger(listener, "width_listener", "Celyn Ward");

                writeLine(speaker, "width " + wideWidth);
                String wide = readUntilQuietAfterContains(speaker, "\r\n> ");
                assertTrue(wide.contains("Display width: 120 columns."), printable(wide));

                writeLine(listener, "width " + narrowWidth);
                String changed = readUntilQuietAfterContains(listener, "\r\n> ");
                assertTrue(changed.contains("Display width: 37 columns."), printable(changed));

                writeLine(listener, "width 19");
                readUntilQuietAfterContains(listener, "\r\n> ");
                writeLine(listener, "width 121");
                readUntilQuietAfterContains(listener, "\r\n> ");
                writeLine(listener, "width");
                String reported = readUntilQuietAfterContains(listener, "\r\n> ");
                assertTrue(reported.contains("Display width: 37 columns."), printable(reported));

                writeLine(speaker, "help");
                String help = readUntilQuietAfterContains(speaker, "\r\n> ");
                assertTrue(help.contains("help settings shows display options"), printable(help));
                writeLine(listener, "help settings");
                String settings = readUntilQuietAfterContains(listener, "\r\n> ");
                assertTrue(settings.contains("Display settings:"), printable(settings));
                assertTrue(settings.replaceAll("\\s+", " ")
                        .contains("width <columns> sets the ruler and wrapping width from 20 to 120."),
                        printable(settings));
                assertWrappedTelnetText(settings, narrowWidth);

                writeLine(listener, "look");
                String look = readUntilQuietAfterContains(listener, "\r\n> ");
                assertTrue(look.contains(narrowRuler), printable(look));
                assertWrappedTelnetText(look, narrowWidth);

                String fitting = "word ".repeat(21) + "words";
                writeLine(speaker, "say " + fitting + " tail");
                String self = readUntilQuietAfterContains(speaker, "\r\n> ");
                String broadcast = readUntilQuietAfterContains(listener, "tail\"\r\n");
                assertTrue(self.contains("You say, \"" + fitting + "\r\ntail\"\r\n"), printable(self));
                assertTrue(broadcast.contains("Arden vale says,"), printable(broadcast));
                assertWrappedTelnetText(self, wideWidth);
                assertWrappedTelnetText(broadcast, narrowWidth);

                String privateMessage = "word ".repeat(17) + "notes";
                writeLine(speaker, "tell Celyn Ward " + privateMessage);
                String confirmation = readUntilQuietAfterContains(speaker, "\r\n> ");
                String delivery = readUntilQuietAfterContains(listener, "notes\"\r\n");
                assertTrue(confirmation.contains("You tell Celyn ward, \"" + privateMessage + "\"\r\n"),
                        printable(confirmation));
                assertTrue(delivery.replaceAll("\\s+", " ")
                        .contains("Arden vale tells you, \"" + privateMessage + "\""), printable(delivery));
                assertWrappedTelnetText(confirmation, wideWidth);
                assertWrappedTelnetText(delivery, narrowWidth);

                writeLine(listener, "quit");
                readUntilSocketClosed(listener);
            }

            try (Socket listener = connect(worker, relaySecret)) {
                listener.setSoTimeout(5000);
                readUntilQuietAfterContains(listener, "Account ID: ");
                writeLine(listener, "width_listener");
                readUntilQuietAfterContains(listener, "Password: ");
                writeLine(listener, "Avelorn1!");
                String returningLook = readUntilQuietAfterContains(listener, "\r\n> ");
                assertTrue(returningLook.contains("Welcome back, Celyn ward."), printable(returningLook));
                assertTrue(returningLook.contains(narrowRuler), printable(returningLook));
                assertWrappedTelnetText(returningLook, narrowWidth);
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

    private void assertWrappedTelnetText(String text, int width) {
        assertTelnetText(text);
        for (String line : text.split("\r\n", -1)) {
            assertTrue(line.length() <= width, printable(text));
        }
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
