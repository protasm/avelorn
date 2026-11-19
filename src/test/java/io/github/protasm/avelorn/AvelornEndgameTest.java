package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;
import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import io.github.protasm.jvmud.language.runtime.RuntimeValueCodec;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Duration;
import java.util.Map;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornEndgameTest {
    private static final String ACCOUNT = "endgame_guest";
    private static final String PASSWORD = "Avelorn1!";
    private static final String MARKET = "place/havenrode/market";
    private static final String QUEST = "an-open-door";
    private static final String HEAVY_STRIKE = "Ledger Enforcer gathers for a heavy strike";

    @TempDir
    Path temporary;

    private MudInstance world;

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Endgame test complete.");
            world = null;
        }
    }

    @Test
    void defeatsTheWeeklyEncounterAndClaimsItsWearableBadgeOnce() throws Exception {
        Path fixture = AvelornFixture.copyTo(temporary.resolve("avelorn"));
        world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        shortenOpeningFerry();
        Client traveler = connect();
        commands(traveler, ACCOUNT, "yes", PASSWORD, PASSWORD, "Vessa Reed", "female", "ranger");
        assertText(traveler.read(), "Westwatch Guest-House");
        world.administer(runtime -> runtime.invokeObject(traveler.session().personaObject(), "gain_experience", 5000));
        assertText(command(traveler, "score"), "level 10 ranger");

        move(traveler, "south", "place/westwatch/landing");
        String routes = command(traveler, "routes");
        assertText(routes, "Blue Lamp Ferry to Havenrode (1 sec, free)");
        assertText(command(traveler, "board Havenrode"), "You board Blue Lamp Ferry");
        awaitFerry(traveler);
        move(traveler, "north east", MARKET);
        assertText(command(traveler, "work " + QUEST), "Complete Under the Mountain first.");
        command(traveler, "save");
        stopWorld();

        seedCompletedFinale(fixture);
        world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        Client hunter = signIn();
        assertEquals(MARKET, room(hunter));
        assertText(command(hunter, "report " + QUEST), "You have not accepted that assignment.");
        assertText(command(hunter, "work " + QUEST), "An Open Door (recommended level 10)");

        move(hunter, "west south out south south enter north", "place/bond-yard/works");
        boolean lockWatch = currentRotation() % 2 == 0;
        String preview = command(hunter, "look enforcer");
        assertText(preview, lockWatch ? "Ledger Enforcer of the Lock Watch" : "Ledger Enforcer of the Debt Sweep");
        assertText(preview, lockWatch ? "short iron shield" : "sledge meant for breaking door bars");
        // Keep the encounter alive long enough to observe the selected heavy cadence.
        assertText(command(hunter, "unequip bow"), "You unequip ashwood shortbow.");
        String combat = defeatEnforcer(hunter);
        int firstHeavy = combat.indexOf(HEAVY_STRIKE);
        assertTrue(firstHeavy >= 0, combat);
        long ordinaryBeforeHeavy = combat.substring(0, firstHeavy).lines()
                .filter(line -> line.contains("Ledger Enforcer closes on")).count();
        assertEquals(lockWatch ? 1 : 3, ordinaryBeforeHeavy, combat);
        assertText(combat, "An Open Door: 1/1");
        assertFalse(combat.contains("You are overcome"), combat);

        move(hunter, "south out north north enter north east", MARKET);
        String reward = command(hunter, "report " + QUEST);
        assertText(reward, "You complete An Open Door.");
        assertText(reward, "You receive open-roof pin.");
        assertText(command(hunter, "equip roof-pin"), "You equip open-roof pin (equipped).");
        assertText(command(hunter, "equipment"), "badge: open-roof pin");
        String paidBalance = command(hunter, "money");
        world.detachSession(hunter.session());

        Client returning = signIn();
        assertText(command(returning, "equipment"), "badge: open-roof pin");
        assertText(command(returning, "report " + QUEST), "already been reported and rewarded");
        assertText(command(returning, "work " + QUEST), "already completed An Open Door");
        assertEquals(paidBalance, command(returning, "money"));
        assertEquals(1, command(returning, "inventory").lines().filter(line -> line.contains("open-roof pin")).count());
    }

    private void seedCompletedFinale(Path fixture) throws IOException {
        // This checkpoint supplies the one prerequisite. Campaign tests earn it through play.
        String journal = RuntimeValueCodec.serialize(Map.of(
                "stages", Map.of("under-the-mountain", 3),
                "counts", Map.of("under-the-mountain", 1),
                "flags", Map.of()));
        ObjectMapper json = new ObjectMapper();
        for (int slot = 0; slot < 2; slot++) {
            Path path = fixture.resolve("accounts/" + ACCOUNT + "." + slot + ".o");
            ObjectNode checkpoint = (ObjectNode) json.readTree(path.toFile());
            ((ObjectNode) checkpoint.at("/fields/persona.adventurer.quest_state")).put("value", journal);
            json.writeValue(path.toFile(), checkpoint);
        }
    }

    @SuppressWarnings("unchecked")
    private void shortenOpeningFerry() {
        world.administer(runtime -> {
            var routes = (Map<?, ?>) runtime.invokeObject(
                    runtime.loadOrGetObject("system/world"), "section", "routes");
            var route = (Map<String, Object>) routes.get("westwatch-havenrode");
            route.put("duration", 1);
            return null;
        });
    }

    private void awaitFerry(Client traveler) throws InterruptedException {
        String destination = "place/havenrode/arrival";
        long deadline = System.nanoTime() + Duration.ofSeconds(3).toNanos();
        while (!destination.equals(room(traveler)) && System.nanoTime() < deadline) {
            Thread.sleep(15);
            world.advanceWorldTick();
        }
        assertEquals(destination, room(traveler), traveler.read());
    }

    private int currentRotation() {
        return world.administer(runtime -> ((Number) runtime.invokeObject(
                runtime.loadOrGetObject("system/quests"), "current_rotation")).intValue());
    }

    private String defeatEnforcer(Client traveler) {
        StringBuilder combat = new StringBuilder(command(traveler, "attack enforcer"));
        assertText(combat.toString(), "You engage Ledger Enforcer");
        for (int tick = 0; tick < 20 && combat.indexOf("Ledger Enforcer is defeated.") < 0; tick++) {
            world.advanceWorldTick();
            combat.append(traveler.read());
        }
        assertText(combat.toString(), "Ledger Enforcer is defeated.");
        return combat.toString();
    }

    private void move(Client traveler, String directions, String target) {
        StringBuilder transcript = new StringBuilder(command(traveler, "walk " + directions));
        for (int tick = 0; tick < 16 && !target.equals(room(traveler)); tick++) {
            world.advanceWorldTick();
            transcript.append(traveler.read());
        }
        assertEquals(target, room(traveler), transcript::toString);
    }

    private Client signIn() {
        Client traveler = connect();
        commands(traveler, ACCOUNT, PASSWORD);
        assertText(traveler.read(), "Welcome back");
        return traveler;
    }

    private Client connect() {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        return new Client(world.attachSession(writer, "127.0.0.1"), writer, output);
    }

    private void commands(Client client, String... lines) {
        for (String line : lines) {
            world.dispatch(client.session(), client.writer(), line);
            world.advanceWorldTick();
        }
    }

    private String command(Client client, String line) {
        commands(client, line);
        return client.read();
    }

    private String room(Client client) {
        return world.administer(runtime -> (String) runtime.invokeObject(
                runtime.loadOrGetObject("system/world"), "place_path",
                runtime.environment(client.session().personaObject())));
    }

    private void assertText(String transcript, String expected) {
        assertTrue(transcript.contains(expected), () -> "Expected: " + expected + "\n" + transcript);
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output) {
        String read() {
            String transcript = output.toString();
            output.getBuffer().setLength(0);
            return transcript;
        }
    }
}
