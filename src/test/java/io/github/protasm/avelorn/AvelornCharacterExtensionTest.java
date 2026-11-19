package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornCharacterExtensionTest {
    private static final String PASSWORD = "Avelorn1!";
    private static final String ORIGINAL_ACCOUNT = "original_fighter";
    private static final String EXPANSION_ACCOUNT = "expansion_keeper";

    @TempDir
    Path tempDir;

    private MudInstance mud;

    @AfterEach
    void stopWorld() {
        if (mud != null) {
            mud.shutdown("Character extension test complete.");
            mud = null;
        }
    }

    @Test
    void extendsPlayableIdentitySkillsAndProgressionAndRestoresBothCharacters() throws Exception {
        Path root = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        startWorld(root);
        assertEquals(List.of("fighter", "ranger", "mage", "cleric"),
                catalog("combat_rules", "classes"));
        assertEquals(List.of("human"), catalog("races", "ids"));
        assertEquals(List.of(), catalog("guilds", "ids"));
        assertEquals(10, catalog("progression", "cap"));

        Client original = connect();
        command(original, ORIGINAL_ACCOUNT, "yes", PASSWORD, PASSWORD, "Alder Vale",
                "non-binary", "male", "fighter");
        String firstLogin = original.read();
        assertText(firstLogin, "Please choose male or female");
        assertText(firstLogin, "Class (fighter/ranger/mage/cleric):");
        assertText(firstLogin, "Welcome to Avelorn");
        assertEquals("human", invoke(original, "query_race"));
        assertText(status(original), "Stamina 49/49");
        invoke(original, "gain_experience", 4500);
        command(original, "south", "save", "quit");
        stopWorld();

        addProfiles(root);
        startWorld(root);
        assertEquals(List.of("fighter", "ranger", "mage", "cleric", "lightkeeper"),
                catalog("combat_rules", "classes"));
        assertEquals(Set.of("human", "shorekin"), Set.copyOf((List<?>) catalog("races", "ids")));
        assertEquals(List.of("lamplighters"), catalog("guilds", "ids"));
        assertEquals(11, catalog("progression", "cap"));
        assertEquals(1000, catalog("progression", "experience_to_advance", 10));
        assertEquals(2, catalog("progression", "training_points_for", 11));

        Client returning = connect();
        command(returning, ORIGINAL_ACCOUNT, PASSWORD);
        assertText(returning.read(), "Westwatch Landing");
        assertEquals("human", invoke(returning, "query_race"));
        assertEquals("fighter", invoke(returning, "query_class"));
        assertEquals("", invoke(returning, "query_guild"));
        assertEquals(10, invoke(returning, "query_level"));
        assertEquals(0, invoke(returning, "join_guild", "lamplighters"));

        // Give the restored character exactly the newly authored next threshold.
        invoke(returning, "gain_experience", 1000);
        invoke(returning, "gain_experience", 1000);
        assertEquals(11, invoke(returning, "query_level"));
        String capped = status(returning);
        assertText(capped, "Experience: level cap reached");
        assertText(capped, "Unspent attribute points: 7");

        Client introduced = connect();
        command(introduced, EXPANSION_ACCOUNT, "yes", PASSWORD, PASSWORD, "Neri Salt", "female");
        String racePrompt = introduced.read();
        assertText(racePrompt, "Race (");
        assertText(racePrompt, "human");
        assertText(racePrompt, "shorekin");
        command(introduced, "shorekin");
        assertText(introduced.read(), "Class (fighter/ranger/mage/cleric/lightkeeper):");
        command(introduced, "lightkeeper", "inventory");
        assertText(introduced.read(), "steel mace (equipped)");
        assertEquals("shorekin", invoke(introduced, "query_race"));
        assertEquals("lightkeeper", invoke(introduced, "query_class"));
        assertTrue(introduced.protocol().stream().anyMatch(message ->
                message.startsWith("Char.Base ") && message.contains("\"race\":\"shorekin\"")));
        String profile = status(introduced);
        assertText(profile, "Wisdom 15");
        assertText(profile, "Dexterity 12");
        assertText(profile, "Focus 58/58");

        command(introduced, "south", "out", "ability steady-lantern mist");
        assertText(introduced.read(), "Choose the lantern to steady.");
        assertEquals(98, invoke(introduced, "query_travel_reserve"));
        assertText(status(introduced), "Focus 58/58");
        command(introduced, "ability steady-lantern the lantern");
        String skill = introduced.read();
        assertText(skill, "The wick steadies at power 5.");
        assertText(skill, "You use Steady Lantern.");
        assertEquals(100, invoke(introduced, "query_travel_reserve"));
        assertText(status(introduced), "Focus 52/58");

        assertEquals(0, invoke(introduced, "join_guild", "unknown"));
        assertEquals(1, invoke(introduced, "join_guild", "lamplighters"));
        assertEquals(0, invoke(introduced, "join_guild", "lamplighters"));
        command(introduced, "save");
        command(returning, "save");
        stopWorld();

        startWorld(root);
        Client restoredOriginal = connect();
        command(restoredOriginal, ORIGINAL_ACCOUNT, PASSWORD);
        assertEquals("human", invoke(restoredOriginal, "query_race"));
        assertEquals("fighter", invoke(restoredOriginal, "query_class"));
        assertEquals(11, invoke(restoredOriginal, "query_level"));
        assertEquals("", invoke(restoredOriginal, "query_guild"));

        Client restoredExpansion = connect();
        command(restoredExpansion, EXPANSION_ACCOUNT, PASSWORD, "abilities");
        assertText(restoredExpansion.read(), "steady-lantern (level 1, 5 Focus)");
        assertEquals("shorekin", invoke(restoredExpansion, "query_race"));
        assertEquals("lightkeeper", invoke(restoredExpansion, "query_class"));
        assertEquals("lamplighters", invoke(restoredExpansion, "query_guild"));
        assertText(status(restoredExpansion), "Focus 52/58");
        assertEquals(1, invoke(restoredExpansion, "leave_guild"));
        assertEquals("", invoke(restoredExpansion, "query_guild"));
    }

    private void startWorld(Path root) {
        mud = MudInstance.boot(root, AvelornFixture.CONFIG_PATH);
    }

    private Client connect() {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        List<String> protocol = new ArrayList<>();
        InstanceSession session = mud.attachSession(writer, "127.0.0.1");
        mud.bindClientProtocolSink(session, (type, message) -> {
            if ("GMCP".equals(type)) {
                protocol.add(message);
            }
        });
        mud.setClientProtocolEnabled(session, "GMCP", true);
        return new Client(session, writer, output, protocol);
    }

    private void command(Client client, String... commands) {
        for (String command : commands) {
            mud.dispatch(client.session(), client.writer(), command);
            mud.advanceWorldTick();
        }
    }

    private String status(Client client) {
        client.read();
        command(client, "score");
        return client.read();
    }

    private Object catalog(String service, String method, Object... arguments) {
        return mud.administer(runtime -> runtime.invokeObject(
                runtime.loadOrGetObject("system/" + service), method, arguments));
    }

    private Object invoke(Client client, String method, Object... arguments) {
        return mud.administer(runtime -> runtime.invokeObject(
                client.session().personaObject(), method, arguments));
    }

    private static void assertText(String transcript, String text) {
        assertTrue(transcript.contains(text), () -> "Expected " + text + "\n" + transcript);
    }

    private void addProfiles(Path root) throws IOException {
        writeSource(root, "class/lightkeeper.c", """
                mapping definition() {
                  return ([
                    "id": "lightkeeper",
                    "name": "Lightkeeper",
                    "primary_stat": "wisdom",
                    "starting_attributes": ([ "wisdom": 15, "dexterity": 12, "constitution": 11 ]),
                    "starter_weapon": "weapon/steel-mace",
                    "resource": ([
                      "name": "Focus", "base": 14, "per_level": 2,
                      "per_stat": ([ "wisdom": 2, "dexterity": 1 ])
                    ]),
                    "style_order": ({ "watch" }),
                    "technique_order": ({ "steady-lantern" }),
                    "styles": ([
                      "watch": ([
                        "name": "Watch", "description": "Keep attention on the light.",
                        "attack": 0, "defense": 1, "cost": 1, "support": 2
                      ])
                    ]),
                    "techniques": ([
                      "steady-lantern": ([
                        "name": "Steady Lantern", "description": "Steady yourself at a flame.",
                        "level": 1, "cost": 5, "effect": "custom", "power": 3,
                        "handler": "skill/steady_lantern"
                      ])
                    ])
                  ]);
                }
                """);
        writeSource(root, "skill/steady_lantern.c", """
                int perform(object actor, string requested, int power) {
                  if (requested != "the lantern") {
                    jvmud_write_to_lpc_object(actor, "Choose the lantern to steady.\\n");
                    return 0;
                  }
                  jvmud_invoke_lpc_object(actor, "adjust_travel_reserve", power);
                  jvmud_write_to_lpc_object(actor, "The wick steadies at power " + power + ".\\n");
                  return 1;
                }
                """);
        writeSource(root, "race/shorekin.c", """
                mapping definition() {
                  return ([ "name": "Shorekin", "description": "Travelers from the outer shore." ]);
                }
                """);
        writeSource(root, "guild/lamplighters.c", """
                mapping definition() {
                  return ([ "name": "Lamplighters", "description": "A fellowship of lightkeepers." ]);
                }

                int eligible(object persona) {
                  return jvmud_invoke_lpc_object(persona, "query_class") == "lightkeeper"
                      && jvmud_invoke_lpc_object(persona, "query_race") == "shorekin";
                }
                """);

        extendSource(root, "class/registry.c", "\"cleric\" });", "\"cleric\", \"lightkeeper\" });");
        extendSource(root, "race/registry.c", "\"human\": \"race/human\"",
                "\"human\": \"race/human\", \"shorekin\": \"race/shorekin\"");
        extendSource(root, "guild/registry.c", "([ ])", "([ \"lamplighters\": \"guild/lamplighters\" ])");
        extendSource(root, "system/progression.c", "({ 900, 1 })   // 10",
                "({ 900, 1 }),  // 10\n    ({ 1000, 2 })  // 11");
    }

    private void extendSource(Path root, String relative, String before, String after) throws IOException {
        String source = Files.readString(root.resolve("source").resolve(relative));
        assertTrue(source.contains(before), relative);
        writeSource(root, relative, source.replace(before, after));
    }

    private void writeSource(Path root, String relative, String source) throws IOException {
        Path path = root.resolve("source").resolve(relative);
        Files.createDirectories(path.getParent());
        Files.writeString(path, source);
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output, List<String> protocol) {
        String read() {
            String transcript = output.toString();
            output.getBuffer().setLength(0);
            return transcript;
        }
    }
}
