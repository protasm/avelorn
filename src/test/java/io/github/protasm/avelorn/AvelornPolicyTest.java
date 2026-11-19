package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornPolicyTest {
    @TempDir
    Path temporary;

    @Test
    void launchProgressionPreservesTheCapThresholdsAndTrainingAwards() throws Exception {
        Path fixture = AvelornFixture.copyTo(temporary.resolve("launch-policy"));
        var world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            world.administer(runtime -> {
                Object progression = runtime.loadOrGetObject("system/progression");
                assertEquals(10, runtime.invokeObject(progression, "cap"));
                for (int level = 1; level < 10; level++) {
                    assertEquals(level * 100, runtime.invokeObject(progression, "experience_to_advance", level));
                }
                for (int level = 1; level <= 10; level++) {
                    assertEquals(level % 2 == 0 ? 1 : 0,
                            runtime.invokeObject(progression, "training_points_for", level));
                }
                for (int outside : List.of(-1, 0, 11)) {
                    assertEquals(0, runtime.invokeObject(progression, "experience_to_advance", outside));
                    assertEquals(0, runtime.invokeObject(progression, "training_points_for", outside));
                }
                assertEquals(0, runtime.invokeObject(progression, "experience_to_advance", 10));
                Object guilds = runtime.loadOrGetObject("system/guilds");
                assertEquals(List.of(), runtime.invokeObject(guilds, "ids"));
                assertEquals(0, runtime.invokeObject(guilds, "known", "bridge-company"));
                return null;
            });
        } finally {
            world.shutdown("End launch policy test.");
        }
    }

    @Test
    void loadsAdditiveGuildsAndPassesThePersonaToOptionalAdmissionRules() throws Exception {
        Path fixture = AvelornFixture.copyTo(temporary.resolve("extended-guilds"));
        Path providers = fixture.resolve("source/guild");
        Files.writeString(providers.resolve("registry.c"), """
                mapping registrations() {
                  return ([
                    "bridge-company": "guild/bridge_company",
                    "shore-watch": "guild/shore_watch"
                  ]);
                }
                """);
        Files.writeString(providers.resolve("bridge_company.c"), """
                mapping definition() {
                  return ([
                    "name": "Bridge Company",
                    "description": "A company that staffs the rope bridges."
                  ]);
                }

                int eligible(object persona) {
                  return jvmud_invoke_lpc_object(persona, "query_class") == "ranger";
                }
                """);
        Files.writeString(providers.resolve("shore_watch.c"), """
                mapping definition() {
                  return ([
                    "name": "Shore Watch",
                    "description": "A watch that keeps the fishing markers lit."
                  ]);
                }
                """);
        var world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            Object ranger = signUp(world, "ranger", "Vessa Reed").personaObject();
            Object fighter = signUp(world, "fighter", "Maren Bell").personaObject();
            world.administer(runtime -> {
                Object guilds = runtime.loadOrGetObject("system/guilds");
                var ids = (List<?>) runtime.invokeObject(guilds, "ids");
                assertEquals(Set.of("bridge-company", "shore-watch"), Set.copyOf(ids));
                assertEquals(1, runtime.invokeObject(guilds, "known", "bridge-company"));
                assertEquals(Map.of("name", "Bridge Company", "description", "A company that staffs the rope bridges."),
                        runtime.invokeObject(guilds, "definition", "bridge-company"));
                assertEquals(1, runtime.invokeObject(guilds, "eligible", "bridge-company", ranger));
                assertEquals(0, runtime.invokeObject(guilds, "eligible", "bridge-company", fighter));
                assertEquals(1, runtime.invokeObject(guilds, "eligible", "shore-watch", fighter));
                assertEquals(0, runtime.invokeObject(guilds, "eligible", "missing", fighter));
                return null;
            });
        } finally {
            world.shutdown("End extended guild test.");
        }
    }

    private InstanceSession signUp(MudInstance world, String characterClass, String name) {
        var output = new StringWriter();
        var writer = new PrintWriter(output, true);
        var session = world.attachSession(writer, "127.0.0.1");
        for (String line : List.of("policy_" + characterClass, "yes", "Avelorn1!", "Avelorn1!",
                name, "female", characterClass)) {
            world.dispatch(session, writer, line);
            world.advanceWorldTick();
        }
        assertTrue(output.toString().contains("Welcome to Avelorn"), output.toString());
        return session;
    }
}
