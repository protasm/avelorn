package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornAtlasScaleTest {
    private static final int REGION_WIDTH = 64;
    private static final int REGION_CACHE_SIZE = 32;
    private static final int REGION_REQUESTS = 40;
    private static final int VISITS_PER_REGION = 8;
    private static final int WESTERN_TIP_ID = 17080434;
    private static final String WESTERN_TIP = "place/atlas/x0113/y3336";
    private static final String ROOM_INFO = "Room.Info ";
    private static final Set<Integer> IMPASSABLE = Set.of(1, 2, 3, 44);

    @TempDir
    Path temporary;

    private final ObjectMapper json = new ObjectMapper();

    @Test
    void evictsPhysicalRegionsAndReusesOnlyUnoccupiedAtlasPlaces() throws Exception {
        Path fixture = AvelornFixture.copyTerrainFixture(temporary.resolve("atlas-scale"));
        List<Region> regions = northIslandRegions(fixture);
        assertEquals(REGION_REQUESTS, regions.size());
        assertEquals(REGION_REQUESTS, Set.copyOf(regions).size());

        var world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            Client watcher = create(world, "atlas_watcher", "Vessa Reed");
            Client traveler = create(world, "atlas_traveler", "Maren Bell");
            List<String> roomInfo = collectRoomInfo(world, watcher);
            JsonNode before = look(world, watcher, roomInfo);
            assertEquals(WESTERN_TIP_ID, before.path("num").asInt());
            assertEquals(WESTERN_TIP_ID + 5120, before.path("exits").path("north").asInt());
            assertEquals(WESTERN_TIP_ID + 1, before.path("exits").path("east").asInt());

            sweepAtlas(world, watcher, traveler, regions);

            assertEquals(before, look(world, watcher, roomInfo));
            assertEquals(WESTERN_TIP_ID, look(world, traveler, collectRoomInfo(world, traveler)).path("num").asInt());
        } finally {
            world.shutdown("Atlas scale test complete.");
        }
    }

    private List<Region> northIslandRegions(Path fixture) throws IOException {
        Path historical = fixture.resolve("atlas/political/pre-conquest");
        JsonNode descriptor = json.readTree(historical.resolve("working-borders.json").toFile());
        Path manifest = historical.resolve(descriptor.path("package").path("path").asText());
        JsonNode packageData = json.readTree(manifest.toFile());
        List<Region> regions = new ArrayList<>();
        for (JsonNode entry : packageData.path("regions")) {
            if (entry.path("square_count").asInt() >= 1024) {
                regions.add(new Region(entry.path("region_x").asInt(), entry.path("region_y").asInt()));
            }
            if (regions.size() == REGION_REQUESTS) {
                break;
            }
        }
        return regions;
    }

    private void sweepAtlas(MudInstance world, Client watcher, Client traveler, List<Region> regions) {
        world.administer(runtime -> {
            Object atlas = runtime.loadOrGetObject("system/atlas");
            Object pool = runtime.loadOrGetObject("system/atlas_places");
            Object worldIndex = runtime.loadOrGetObject("system/world");
            Object stationary = watcher.session().personaObject();
            Object moving = traveler.session().personaObject();
            Object westernTip = runtime.environment(stationary);
            assertSame(westernTip, runtime.environment(moving));
            assertEquals(WESTERN_TIP, runtime.invokeObject(worldIndex, "place_path", westernTip));

            var loaded = new ArrayList<Map<?, ?>>();
            var squares = new ArrayList<Square>();
            for (Region region : regions) {
                var records = (Map<?, ?>) runtime.invokeObject(atlas, "load_region", region.x(), region.y());
                assertFalse(records.isEmpty(), region.toString());
                loaded.add(records);
                List<Square> sample = records.values().stream()
                        .map(value -> (List<?>) value)
                        .filter(row -> !IMPASSABLE.contains(((Number) row.get(2)).intValue()))
                        .map(row -> region.square(((Number) row.get(0)).intValue()))
                        .limit(VISITS_PER_REGION)
                        .toList();
                assertEquals(VISITS_PER_REGION, sample.size(), region.toString());
                squares.addAll(sample);
            }

            int oldestRetained = REGION_REQUESTS - REGION_CACHE_SIZE;
            Region retained = regions.get(oldestRetained);
            assertSame(loaded.get(oldestRetained),
                    runtime.invokeObject(atlas, "load_region", retained.x(), retained.y()));
            Region evicted = regions.get(oldestRetained - 1);
            Object reloaded = runtime.invokeObject(atlas, "load_region", evicted.x(), evicted.y());
            assertNotSame(loaded.get(oldestRetained - 1), reloaded);
            assertEquals(loaded.get(oldestRetained - 1), reloaded);

            Set<Object> reused = Collections.newSetFromMap(new IdentityHashMap<>());
            for (Square square : squares) {
                assertEquals(1, runtime.invokeObject(worldIndex, "relocate", moving, square.path()), square.path());
                Object place = runtime.environment(moving);
                reused.add(place);
                assertNotSame(westernTip, place);
                assertEquals(square.path(), runtime.invokeObject(worldIndex, "place_path", place));
                assertEquals(square.gmcpId(), runtime.invokeObject(place, "query_gmcp_id"));
            }
            assertTrue(reused.size() <= 2, "Expected reusable moving slots across " + squares.size() + " squares.");
            assertTrue(((Number) runtime.invokeObject(pool, "room_count")).intValue() <= 3);
            assertSame(westernTip, runtime.environment(stationary));
            assertEquals(WESTERN_TIP, runtime.invokeObject(worldIndex, "place_path", westernTip));
            assertEquals(WESTERN_TIP_ID, runtime.invokeObject(westernTip, "query_gmcp_id"));

            Square firstVisited = squares.getFirst();
            assertEquals(1, runtime.invokeObject(worldIndex, "relocate", moving, firstVisited.path()));
            assertEquals(firstVisited.gmcpId(), runtime.invokeObject(runtime.environment(moving), "query_gmcp_id"));
            assertEquals(1, runtime.invokeObject(worldIndex, "relocate", moving, WESTERN_TIP));
            assertSame(westernTip, runtime.environment(moving));
            return null;
        });
    }

    private List<String> collectRoomInfo(MudInstance world, Client client) {
        List<String> roomInfo = new ArrayList<>();
        world.bindClientProtocolSink(client.session(), (protocol, message) -> {
            if (protocol.equals("GMCP") && message.startsWith(ROOM_INFO)) {
                roomInfo.add(message.substring(ROOM_INFO.length()));
            }
        });
        world.setClientProtocolEnabled(client.session(), "GMCP", true);
        return roomInfo;
    }

    private JsonNode look(MudInstance world, Client client, List<String> roomInfo) throws IOException {
        roomInfo.clear();
        world.dispatch(client.session(), client.writer(), "look");
        world.advanceWorldTick();
        assertFalse(roomInfo.isEmpty());
        return json.readTree(roomInfo.getLast());
    }

    private Client create(MudInstance world, String account, String name) {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstanceSession session = world.attachSession(writer, "127.0.0.1");
        for (String line : List.of(account, "yes", "Avelorn1!", "Avelorn1!", name, "female", "ranger")) {
            world.dispatch(session, writer, line);
            world.advanceWorldTick();
        }
        assertTrue(output.toString().contains("Welcome to Avelorn"), output.toString());
        return new Client(session, writer);
    }

    private record Client(InstanceSession session, PrintWriter writer) {}

    private record Region(int x, int y) {
        Square square(int localIndex) {
            return new Square(x * REGION_WIDTH + localIndex % REGION_WIDTH,
                    y * REGION_WIDTH + localIndex / REGION_WIDTH);
        }
    }

    private record Square(int x, int y) {
        String path() {
            return String.format(Locale.ROOT, "place/atlas/x%04d/y%04d", x, y);
        }

        int gmcpId() {
            return 1 + y * 5120 + x;
        }
    }
}
