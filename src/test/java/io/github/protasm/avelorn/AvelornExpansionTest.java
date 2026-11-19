package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornExpansionTest {
    private static final String ACCOUNT = "croft_visitor";
    private static final String PASSWORD = "Avelorn1!";
    private static final String LANDING = "place/westwatch/landing";
    private static final String LAMP_QUEST = "lamp-on-the-headland";
    private static final String ROOM_INFO = "Room.Info ";

    @TempDir
    Path tempDir;

    private final ObjectMapper json = new ObjectMapper();
    private MudInstance mud;

    @AfterEach
    void stopWorld() {
        if (mud != null) {
            mud.shutdown("Expansion test complete.");
            mud = null;
        }
    }

    @Test
    void addsTwoLpcAreasAndPreservesTheSavedPlaceGmcpAndOriginalQuest() throws Exception {
        Path root = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        startWorld(root);
        Client original = connect();
        command(original, ACCOUNT, "yes", PASSWORD, PASSWORD, "Neri Vale", "female", "ranger");
        command(original, "work " + LAMP_QUEST, "south", "save");
        assertEquals(LANDING, place(original));
        assertEquals(1, lampQuestStage(original));
        JsonNode oldRoom = room(original);
        assertEquals(30000002, oldRoom.path("num").asInt());
        command(original, "quit");
        stopWorld();

        addLpcAreas(root);
        startWorld(root);
        Client restored = connect();
        command(restored, ACCOUNT, PASSWORD);
        assertEquals(LANDING, place(restored));
        assertEquals(oldRoom, room(restored));
        assertEquals(1, lampQuestStage(restored));

        command(restored, "out", "north");
        restored.read();
        command(restored, "enter");
        assertEquals("place/windward-croft/bench", place(restored));
        assertEquals(30008000, room(restored).path("num").asInt());
        String arrival = restored.read();
        assertTrue(arrival.contains("A lamp stands on a tripod."), arrival);
        assertFalse(arrival.contains("[lamp]"), arrival);

        command(restored, "details");
        String details = restored.read();
        assertTrue(details.contains("lamp"), details);
        assertFalse(details.contains("kettle"), details);

        command(restored, "hints on", "look");
        String hinted = restored.read();
        assertTrue(hinted.contains("A [lamp] stands on a tripod."), hinted);
        command(restored, "exa lamp");
        String lamp = restored.read();
        assertTrue(lamp.contains("A glass pane shields the flame from spray."), lamp);
        command(restored, "exa kettle");
        String kettle = restored.read();
        assertTrue(kettle.contains("You do not see that here."), kettle);

        command(restored, "out", "east", "east");
        restored.read();
        command(restored, "enter", "exa shell");
        assertEquals("place/tidecut-cave/ledge", place(restored));
        assertEquals(30008001, room(restored).path("num").asInt());
        String cave = restored.read();
        assertTrue(cave.contains("Tidecut Ledge"), cave);
        assertTrue(cave.contains("Old mussel valves are cemented to the rock."), cave);

        // Restore the interrupted Westwatch assignment through its existing combat and turn-in.
        command(restored, "out", "west", "west", "south", "east", "enter");
        assertEquals("place/headland-signal/lamp-walk", place(restored));
        command(restored, "attack wolf");
        for (int tick = 0; tick < 20 && lampQuestStage(restored) < 2; tick++) {
            mud.advanceWorldTick();
        }
        assertEquals(2, lampQuestStage(restored), restored.read());
        command(restored, "out", "west", "enter", "north", "report " + LAMP_QUEST);
        assertEquals(3, lampQuestStage(restored), restored.read());
        assertEquals("place/westwatch/guest-house", place(restored));
    }

    private void startWorld(Path root) {
        mud = MudInstance.boot(root, AvelornFixture.CONFIG_PATH);
    }

    private Client connect() {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        List<String> rooms = new ArrayList<>();
        InstanceSession session = mud.attachSession(writer, "127.0.0.1");
        mud.bindClientProtocolSink(session, (protocol, message) -> {
            if ("GMCP".equals(protocol) && message.startsWith(ROOM_INFO)) {
                rooms.add(message.substring(ROOM_INFO.length()));
            }
        });
        mud.setClientProtocolEnabled(session, "GMCP", true);
        return new Client(session, writer, output, rooms);
    }

    private void command(Client client, String... commands) {
        for (String command : commands) {
            mud.dispatch(client.session(), client.writer(), command);
            mud.advanceWorldTick();
        }
    }

    private String place(Client client) {
        return mud.administer(runtime -> (String) runtime.invokeObject(
                runtime.loadOrGetObject("system/world"), "place_path",
                runtime.environment(client.session().personaObject())));
    }

    private int lampQuestStage(Client client) {
        return mud.administer(runtime -> ((Number) runtime.invokeObject(
                client.session().personaObject(), "quest_stage", LAMP_QUEST)).intValue());
    }

    private JsonNode room(Client client) throws IOException {
        return json.readTree(client.rooms().getLast());
    }

    private void addLpcAreas(Path root) throws IOException {
        Path registry = root.resolve("source/area/registry.c");
        String existing = Files.readString(registry);
        String areasDeclaration = "mixed *areas() {\n  return ({\n";
        assertTrue(existing.contains(areasDeclaration));
        Files.writeString(registry, existing.replace(areasDeclaration,
                areasDeclaration + "    \"windward-croft\", \"tidecut-cave\",\n"));

        writeSource(root, "area/windward-croft/area.c", """
                mapping definition() {
                  return ([
                    "name": "Windward Croft",
                    "nation": "arvenna",
                    "kind": "village",
                    "anchor": ({ 113, 3337 }),
                    "entrance": "place/windward-croft/bench",
                    "control": "contested",
                    "institution": "Coastal households keep a bench open for travelers.",
                    "economy": "Dried herring and spare lamp glass.",
                    "rooms": ([
                      "place/windward-croft/bench": ({ 30008000, "settlement" })
                    ])
                  ]);
                }
                """);
        writeSource(root, "place/windward-croft/bench.c", """
                inherit "system/place";

                mapping definition() {
                  return ([
                    "name": "The Croft Bench",
                    "description": "A [lamp] stands on a tripod. An old kettle rattles on its hook.",
                    "environment": "settlement",
                    "exits": ([ "out": "place/atlas/x0113/y3337" ]),
                    "services": ({ "rest" }),
                    "npcs": ({ }),
                    "safe": 1,
                    "details": ([ "lamp": "A glass pane shields the flame from spray." ])
                  ]);
                }
                """);
        writeSource(root, "area/tidecut-cave/area.c", """
                mapping definition() {
                  return ([
                    "name": "Tidecut Cave",
                    "nation": "arvenna",
                    "kind": "site",
                    "anchor": ({ 115, 3337 }),
                    "entrance": "place/tidecut-cave/ledge",
                    "control": "contested",
                    "institution": "Pilots use the ledge to wait out squalls.",
                    "economy": "Dry shelter for crews caught on the headland.",
                    "rooms": ([
                      "place/tidecut-cave/ledge": ({ 30008001, "interior" })
                    ]),
                    "site": ([
                      "area": "tidecut-cave",
                      "level": 1,
                      "summary": "Find the dry ledge and examine the shell deposits."
                    ])
                  ]);
                }
                """);
        writeSource(root, "place/tidecut-cave/ledge.c", """
                inherit "system/place";

                mapping definition() {
                  return ([
                    "name": "Tidecut Ledge",
                    "description": "A [shell] catches the light above a tide-cut channel.",
                    "environment": "interior",
                    "exits": ([ "out": "place/atlas/x0115/y3337" ]),
                    "services": ({ }),
                    "npcs": ({ }),
                    "safe": 1,
                    "details": ([ "shell": "Old mussel valves are cemented to the rock." ])
                  ]);
                }
                """);
    }

    private void writeSource(Path root, String relative, String source) throws IOException {
        Path path = root.resolve("source").resolve(relative);
        Files.createDirectories(path.getParent());
        Files.writeString(path, source);
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output, List<String> rooms) {
        String read() {
            String result = output.toString();
            output.getBuffer().setLength(0);
            return result;
        }
    }
}
