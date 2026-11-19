package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;
import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.Map;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornTravelTest {
    private static final String PASSWORD = "Avelorn1!";
    private static final String GUEST_HOUSE = "place/westwatch/guest-house";
    private static final String LANDING = "place/westwatch/landing";
    private static final String DUNES = "place/atlas/x0113/y3336";
    private static final String NORTH_DUNES = "place/atlas/x0113/y3337";

    @TempDir
    Path tempDir;

    private final ObjectMapper json = new ObjectMapper();
    private MudInstance world;

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Travel test complete.");
            world = null;
        }
    }

    @Test
    void batchesSettledMovementAndChargesOnlySuccessfulWildernessSteps() throws Exception {
        startWorld(AvelornFixture.copyTo(tempDir.resolve("avelorn")));
        Client traveler = create("travel_walk", "Arden Vale");

        send(traveler, "walk east west east west south out north");
        assertEquals(GUEST_HOUSE, room(traveler));
        world.advanceWorldTick();
        assertEquals(GUEST_HOUSE, room(traveler));
        assertEquals(3, queued(traveler, "travel"));
        assertEquals(100, reserve(traveler));

        world.advanceWorldTick();
        assertEquals(LANDING, room(traveler));
        assertEquals(2, queued(traveler, "travel"));
        assertEquals(100, reserve(traveler));

        world.advanceWorldTick();
        assertEquals(DUNES, room(traveler));
        assertEquals(1, queued(traveler, "travel"));
        assertEquals(98, reserve(traveler));

        world.advanceWorldTick();
        assertEquals(NORTH_DUNES, room(traveler));
        assertEquals(0, queued(traveler, "travel"));
        assertEquals(96, reserve(traveler));

        send(traveler, "walk west south");
        world.advanceWorldTick();
        assertEquals(NORTH_DUNES, room(traveler));
        assertEquals(0, queued(traveler, "travel"));
        assertEquals(96, reserve(traveler));
    }

    @Test
    void aLargePasteIsBoundedAndAnotherPlayerCanStillAct() throws Exception {
        startWorld(AvelornFixture.copyTo(tempDir.resolve("avelorn")));
        Client traveler = create("travel_paste", "Arden Vale");
        Client friend = create("travel_friend", "Mira Vale");
        send(traveler, "walk south out");
        world.advanceWorldTick();
        world.advanceWorldTick();
        assertEquals(DUNES, room(traveler));
        traveler.read();

        for (int index = 0; index < 10_000; index++) {
            send(traveler, "north");
        }
        assertEquals(16, queued(traveler, "input"));
        assertEquals(DUNES, room(traveler));
        String overflow = traveler.read();
        assertTrue(overflow.contains("Command queue full."), overflow);

        send(friend, "who", "east");
        world.advanceWorldTick();
        assertEquals(NORTH_DUNES, room(traveler));
        assertEquals(10, queued(traveler, "input"));
        assertEquals(5, queued(traveler, "travel"));
        assertEquals("place/westwatch/coopers-yard", room(friend));
        String presence = friend.read();
        assertTrue(presence.contains("Adventurers (2 online)"), presence);

        send(traveler, "stop");
        assertEquals(0, queued(traveler, "input"));
        assertEquals(0, queued(traveler, "travel"));
        world.advanceWorldTick();
        assertEquals(NORTH_DUNES, room(traveler));
    }

    @Test
    void campsOnlyAfterTheRestPeriodAndConsumesOneRation() throws Exception {
        startWorld(AvelornFixture.copyTo(tempDir.resolve("avelorn")));
        Client traveler = create("travel_camp", "Arden Vale");
        send(traveler, "east");
        world.advanceWorldTick();
        send(traveler, "buy ration");
        world.advanceWorldTick();
        send(traveler, "walk west south out");
        world.advanceWorldTick();
        world.advanceWorldTick();
        assertEquals(DUNES, room(traveler));
        assertEquals(98, reserve(traveler));

        send(traveler, "camp");
        world.advanceWorldTick();
        String start = traveler.read();
        assertTrue(start.contains("You make camp."), start);
        for (int index = 0; index < 14; index++) {
            world.advanceWorldTick();
        }
        assertEquals(98, reserve(traveler));
        assertFalse(traveler.read().contains("You eat your ration"));

        world.advanceWorldTick();
        assertEquals(100, reserve(traveler));
        String complete = traveler.read();
        assertTrue(complete.contains("You eat your ration and recover."), complete);
        assertEquals(0, invoke(traveler, "query_has_travel_ration"));
    }

    @Test
    void restoresAMidVoyageCheckpointAndArrivesWithoutChargingAgain() throws Exception {
        String account = "travel_ferry";
        Path original = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        startWorld(original);
        // Standard services are free. A fixture fare makes duplicate debits observable.
        changeFixtureFare("westwatch-havenrode", 5);
        Client traveler = create(account, "Arden Vale");
        int originalCopper = durableField(original, account, "copper");
        send(traveler, "south");
        world.advanceWorldTick();
        send(traveler, "routes");
        world.advanceWorldTick();
        String departures = traveler.read();
        assertTrue(departures.contains("Blue Lamp Ferry to Havenrode"), departures);

        send(traveler, "board Havenrode", "board westwatch-havenrode", "north");
        world.advanceWorldTick();
        assertEquals("place/in-transit", room(traveler));
        assertEquals(originalCopper - 5, durableField(original, account, "copper"));
        assertEquals(0, queued(traveler, "input"));
        assertEquals(0, queued(traveler, "travel"));
        String board = traveler.read();
        assertTrue(board.contains("You board Blue Lamp Ferry for Havenrode."), board);

        Path recovered = AvelornFixture.copyTo(tempDir.resolve("restart"));
        Path checkpoints = recovered.resolve("accounts");
        Files.createDirectory(checkpoints);
        try (var saves = Files.list(original.resolve("accounts"))) {
            for (Path slot : saves.toList()) {
                JsonNode snapshot = json.readTree(slot.toFile());
                JsonNode deadline = field(snapshot, "transit_arrives_at");
                if (deadline != null && deadline.path("value").asInt() > 0) {
                    ((ObjectNode) deadline).put("value", 1);
                }
                json.writeValue(checkpoints.resolve(slot.getFileName()).toFile(), snapshot);
            }
        }

        stopWorld();
        startWorld(recovered);
        Client returning = connect();
        send(returning, account, PASSWORD);
        world.advanceWorldTick();
        assertEquals("place/havenrode/arrival", room(returning));
        assertEquals(originalCopper - 5, durableField(recovered, account, "copper"));
        assertEquals(0, durableField(recovered, account, "transit_arrives_at"));
        String arrival = returning.read();
        assertTrue(arrival.contains("You arrive at Havenrode."), arrival);

        world.advanceWorldTick();
        assertFalse(returning.read().contains("You arrive at Havenrode."));
        assertEquals(originalCopper - 5, durableField(recovered, account, "copper"));
    }

    @SuppressWarnings("unchecked")
    private void changeFixtureFare(String id, int fare) {
        world.administer(runtime -> {
            var routes = (Map<?, ?>) runtime.invokeObject(
                    runtime.loadOrGetObject("system/world"), "section", "routes");
            var route = (Map<String, Object>) routes.get(id);
            route.put("fare", fare);
            return null;
        });
    }

    private void startWorld(Path root) {
        world = MudInstance.boot(root, AvelornFixture.CONFIG_PATH);
    }

    private Client connect() {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        return new Client(world.attachSession(writer, "127.0.0.1"), writer, output);
    }

    private Client create(String account, String name) {
        Client client = connect();
        send(client, account, "yes", PASSWORD, PASSWORD, name, "female", "ranger");
        String transcript = client.read();
        assertTrue(transcript.contains("Welcome to Avelorn"), transcript);
        return client;
    }

    private void send(Client client, String... lines) {
        for (String line : lines) {
            world.dispatch(client.session(), client.writer(), line);
        }
    }

    private String room(Client client) {
        return world.administer(runtime -> (String) runtime.invokeObject(
                runtime.loadOrGetObject("system/world"), "place_path",
                runtime.environment(client.session().personaObject())));
    }

    private int reserve(Client client) {
        return invoke(client, "query_travel_reserve");
    }

    private int invoke(Client client, String method) {
        return world.administer(runtime -> ((Number) runtime.invokeObject(
                client.session().personaObject(), method)).intValue());
    }

    private int queued(Client client, String service) {
        return world.administer(runtime -> ((Number) runtime.invokeObject(
                runtime.loadOrGetObject("system/" + service), "count", client.session().personaObject())).intValue());
    }

    private int durableField(Path root, String account, String name) throws IOException {
        try (var slots = Files.list(root.resolve("accounts"))) {
            var snapshots = new java.util.ArrayList<JsonNode>();
            for (Path slot : slots.filter(path -> path.getFileName().toString().startsWith(account + ".")).toList()) {
                snapshots.add(json.readTree(slot.toFile()));
            }
            JsonNode latest = snapshots.stream()
                    .max(Comparator.comparingInt(snapshot -> field(snapshot, "snapshot_generation").path("value").asInt()))
                    .orElseThrow();
            return field(latest, name).path("value").asInt();
        }
    }

    private JsonNode field(JsonNode snapshot, String name) {
        var entries = snapshot.path("fields").fields();
        while (entries.hasNext()) {
            var entry = entries.next();
            if (entry.getKey().endsWith("." + name)) {
                return entry.getValue();
            }
        }
        return null;
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output) {
        String read() {
            String transcript = output.toString();
            output.getBuffer().setLength(0);
            return transcript;
        }
    }
}
