package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.function.UnaryOperator;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornAccountTest {
    private static final String PASSWORD = "Avelorn1!";
    private static final String ACCOUNT = "test_guest";

    @TempDir
    Path tempDir;

    private Path mudlib;
    private MudInstance world;

    @BeforeEach
    void startWorld() throws Exception {
        mudlib = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        world = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
    }

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Test finished.");
        }
    }

    @Test
    void deniesASecondActiveSessionAndRestoresProgressWhenTheFirstLeaves() {
        Client first = createCharacter();
        Client second = connect();

        commands(second, ACCOUNT, PASSWORD);
        assertTrue(second.transcript().contains("That account is already playing."), second.transcript());
        assertFalse(second.transcript().contains("Welcome back"), second.transcript());

        commands(first, "drop draught");
        assertTrue(first.transcript().contains("You drop minor healing draught."), first.transcript());
        world.detachSession(first.session());

        second.output().getBuffer().setLength(0);
        commands(second, ACCOUNT, PASSWORD);
        assertTrue(second.transcript().contains("Welcome back, Neri vale."), second.transcript());
        second.output().getBuffer().setLength(0);
        commands(second, "inventory");
        assertTrue(second.transcript().contains("ashwood shortbow (equipped)"), second.transcript());
        assertFalse(second.transcript().contains("minor healing draught"), second.transcript());
    }

    @Test
    void leavesBothSavesUnchangedOnUnauthenticatedDisconnects() throws Exception {
        commands(createCharacter(), "save", "quit");
        Path older = checkpoint(mudlib, ACCOUNT, 0);
        Path newer = checkpoint(mudlib, ACCOUNT, 1);
        byte[] olderBefore = Files.readAllBytes(older);
        byte[] newerBefore = Files.readAllBytes(newer);

        Client guest = connect();
        commands(guest, ACCOUNT);
        assertTrue(guest.transcript().contains("Password: "), guest.transcript());
        world.detachSession(guest.session());

        Client wrong = connect();
        commands(wrong, ACCOUNT, "Wrong1!");
        assertTrue(wrong.transcript().contains("That password did not match."), wrong.transcript());
        world.detachSession(wrong.session());

        assertArrayEquals(olderBefore, Files.readAllBytes(older));
        assertArrayEquals(newerBefore, Files.readAllBytes(newer));
    }

    @Test
    void migratesARealVersionOneSnapshotAfterAuthentication() throws Exception {
        Path legacy = mudlib.resolve("accounts/legacy_guest.o");
        Files.createDirectories(legacy.getParent());
        // This synthetic account was created by the original mudlib at 78774a4.
        try (var snapshot = getClass().getResourceAsStream("/legacy/legacy_guest.o")) {
            assertNotNull(snapshot);
            Files.copy(snapshot, legacy);
        }
        byte[] original = Files.readAllBytes(legacy);

        Client returning = connect();
        commands(returning, "legacy_guest", PASSWORD);
        assertTrue(returning.transcript().contains("Choose your character's gender (male/female): "),
                returning.transcript());
        assertFalse(Files.exists(checkpoint(mudlib, "legacy_guest", 1)));

        commands(returning, "non-binary", "female", "score", "inventory", "journal", "pronouns", "quit");
        String transcript = returning.transcript();
        assertTrue(transcript.contains("Please choose male or female"), transcript);
        assertTrue(transcript.contains("Welcome back, First guest."), transcript);
        assertTrue(transcript.contains("level 3 ranger"), transcript);
        assertTrue(transcript.contains("Experience 15/"), transcript);
        assertTrue(transcript.contains("Dexterity 15"), transcript);
        assertTrue(transcript.contains("ashwood shortbow (equipped)"), transcript);
        assertTrue(transcript.contains("Your journal contains no assignments."), transcript);
        assertTrue(transcript.contains("uses she/her pronouns"), transcript);

        assertTrue(Files.readString(checkpoint(mudlib, "legacy_guest", 1))
                .contains("place/atlas/x0113/y3337"));
        assertArrayEquals(original, Files.readAllBytes(legacy));
        Client again = connect();
        commands(again, "legacy_guest", PASSWORD, "score");
        assertTrue(again.transcript().contains("Welcome back, First guest."), again.transcript());
        assertFalse(again.transcript().contains("Choose your character's gender"), again.transcript());
    }

    @Test
    void resumesThePriorCheckpointAfterTheLatestWriteIsTruncated() throws Exception {
        verifyCheckpointRecovery(checkpoint -> checkpoint.substring(0, checkpoint.length() / 2));
    }

    @Test
    void resumesThePriorCheckpointAfterTheLatestWriteContainsAnInvalidClass() throws Exception {
        verifyCheckpointRecovery(checkpoint -> checkpoint.replace("\"value\" : \"ranger\"",
                "\"value\" : \"unknown-class\""));
    }

    private void verifyCheckpointRecovery(UnaryOperator<String> damage) throws Exception {
        Client first = createCharacter();
        commands(first, "save", "drop draught");
        Path older = checkpoint(mudlib, ACCOUNT, 0);
        Path newer = checkpoint(mudlib, ACCOUNT, 1);
        assertTrue(Files.readString(older).contains("consumable/healing-draught"));
        String latest = Files.readString(newer);
        assertFalse(latest.contains("consumable/healing-draught"));
        String damaged = damage.apply(latest);
        assertFalse(damaged.equals(latest));

        // Take the two files visible at the moment of interruption, before any clean shutdown saves.
        Path restart = AvelornFixture.copyTo(tempDir.resolve("restart"));
        Files.createDirectories(restart.resolve("accounts"));
        Files.copy(older, checkpoint(restart, ACCOUNT, 0));
        Files.writeString(checkpoint(restart, ACCOUNT, 1), damaged);
        stopWorld();
        world = MudInstance.boot(restart, AvelornFixture.CONFIG_PATH);

        Client recovered = connect();
        commands(recovered, ACCOUNT, PASSWORD, "inventory");
        String transcript = recovered.transcript();
        assertTrue(transcript.contains("Welcome back, Neri vale."), transcript);
        assertTrue(transcript.contains("minor healing draught"), transcript);
        assertTrue(Files.readString(checkpoint(restart, ACCOUNT, 1)).contains("consumable/healing-draught"));
    }

    private Path checkpoint(Path root, String account, int slot) {
        return root.resolve("accounts/" + account + "." + slot + ".o");
    }

    private Client createCharacter() {
        Client client = connect();
        commands(client, ACCOUNT, "yes", PASSWORD, PASSWORD, "Neri Vale", "male", "ranger");
        assertTrue(client.transcript().contains("Welcome to Avelorn, Neri vale."), client.transcript());
        return client;
    }

    private Client connect() {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        return new Client(world.attachSession(writer, "127.0.0.1"), output, writer);
    }

    private void commands(Client client, String... lines) {
        for (String line : lines) {
            world.dispatch(client.session(), client.writer(), line);
            world.advanceWorldTick();
        }
    }

    private record Client(InstanceSession session, StringWriter output, PrintWriter writer) {
        String transcript() {
            return output.toString();
        }
    }
}
