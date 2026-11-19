package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornItemExtensionTest {
    @TempDir
    Path tempDir;

    @Test
    void anAuthoredItemCanAddAUseAndEquipmentSlotThroughTheNormalCommands() throws Exception {
        Path mudlib = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        Path sources = mudlib.resolve("source");
        Path catalog = sources.resolve("system/items.c");
        String registration = "blueprint_ids = ({";
        String original = Files.readString(catalog);
        assertTrue(original.contains(registration));
        Files.writeString(catalog, original.replace(registration,
                registration + "\n    \"tool/sounding-line\","));
        Path program = sources.resolve("item/tool/sounding-line.c");
        Files.createDirectories(program.getParent());
        Files.writeString(program, """
                inherit "/item/item";

                string query_blueprint() { return "tool/sounding-line"; }

                mapping item_profile() {
                  return ([
                    "name": "sounding line", "alias": "line",
                    "description": "Knots mark the fathoms along the cord.",
                    "slot": "instrument", "weight": 1, "value": 0,
                    "level": 1, "stat": "none", "minimum": 0
                  ]);
                }

                int use_by(object actor) {
                  avelorn_write_to(actor, "You measure the depth with the sounding line.\\n");
                  return 1;
                }
                """);

        MudInstance world = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        try {
            var output = new StringWriter();
            var writer = new PrintWriter(output, true);
            InstanceSession session = world.attachSession(writer, "127.0.0.1");
            commands(world, session, writer, "soundingtest", "yes", "Avelorn1!", "Avelorn1!",
                    "Neri Vale", "female", "ranger");
            Object actor = session.personaObject();
            world.administer(runtime -> {
                runtime.invokeObject(actor, "purchase_blueprint", "tool/sounding-line");
                runtime.invokeObject(actor, "purchase_blueprint", "consumable/cistern-water");
                runtime.invokeObject(actor, "adjust_travel_reserve", -20);
                runtime.invokeObject(actor, "receive_damage", 12, "A staged bruise");
                return null;
            });
            output.getBuffer().setLength(0);

            commands(world, session, writer, "equip line", "use line", "use line", "use water",
                    "use water", "use draught", "equipment", "inventory");
            String transcript = output.toString();
            String used = "You measure the depth with the sounding line.";
            assertTrue(transcript.indexOf(used) >= 0
                    && transcript.indexOf(used) < transcript.lastIndexOf(used), transcript);
            assertTrue(transcript.contains("instrument: sounding line (equipped)"), transcript);
            assertTrue(transcript.contains("Travel reserve 96/100."), transcript);
            assertTrue(transcript.contains("You are not carrying that item."), transcript);
            assertTrue(transcript.contains("You use minor healing draught."), transcript);
            assertTrue(transcript.contains("You recover ") && transcript.contains(" health."), transcript);
            int reserve = world.administer(runtime ->
                    ((Number) runtime.invokeObject(actor, "query_travel_reserve")).intValue());
            assertEquals(96, reserve);
            assertTrue(transcript.contains("You are carrying:"), transcript);
            String inventory = transcript.substring(transcript.indexOf("You are carrying:"));
            assertFalse(inventory.contains("cistern water") || inventory.contains("healing draught"), transcript);
        } finally {
            world.shutdown("Test finished.");
        }
    }

    private void commands(MudInstance world, InstanceSession session, PrintWriter writer, String... lines) {
        for (String line : lines) {
            world.dispatch(session, writer, line);
            world.advanceWorldTick();
        }
    }
}
