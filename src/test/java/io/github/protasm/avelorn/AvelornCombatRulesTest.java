package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertInstanceOf;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornCombatRulesTest {
    @TempDir
    Path tempDir;

    private MudInstance world;

    @BeforeEach
    void startWorld() throws Exception {
        Path mudlib = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        world = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
    }

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Test finished.");
        }
    }

    @Test
    void materializesCombatProfilesAndPreservesTheLegacyItemAliases() {
        world.administer(runtime -> {
            Object catalog = runtime.loadOrGetObject("system/items");
            List<?> ids = assertInstanceOf(List.class, runtime.invokeObject(catalog, "ids"));
            assertTrue(ids.size() >= 45);
            for (Object entry : ids) {
                String id = assertInstanceOf(String.class, entry);
                Map<?, ?> profile = assertInstanceOf(Map.class, runtime.invokeObject(catalog, "blueprint", id));
                Object item = runtime.invokeObject(catalog, "create", id);
                assertTrue(runtime.objectId(item).startsWith("item/" + id + "#"), id);
                assertEquals(id, runtime.invokeObject(item, "query_blueprint"));
                assertEquals(profile.get("name"), runtime.invokeObject(item, "short"));
                assertEquals(profile.get("slot"), runtime.invokeObject(item, "query_slot"));
                assertEquals(profile.get("value"), runtime.invokeObject(item, "query_value"));
            }
            Object sword = runtime.invokeObject(catalog, "create", "weapon/crown-arming-sword");
            assertEquals("weapon/arming-sword", runtime.invokeObject(sword, "query_blueprint"));
            Object anotherSword = runtime.invokeObject(catalog, "create", "weapon/arming-sword");
            assertNotSame(sword, anotherSword);
            runtime.invokeObject(sword, "set_equipped", 1);
            assertEquals(1, runtime.invokeObject(sword, "query_equipped"));
            assertEquals(0, runtime.invokeObject(anotherSword, "query_equipped"));
            assertEquals(4, runtime.invokeObject(sword, "query_power"));
            assertEquals("cut", runtime.invokeObject(sword, "query_damage_type"));
            Object mace = runtime.invokeObject(catalog, "create", "weapon/temple-mace");
            assertEquals("weapon/steel-mace", runtime.invokeObject(mace, "query_blueprint"));
            Object coat = runtime.invokeObject(catalog, "create", "armor/hearing-coat");
            assertEquals("body", runtime.invokeObject(coat, "query_slot"));
            assertEquals(4, runtime.invokeObject(coat, "query_protection"));
            Object bread = runtime.invokeObject(catalog, "create", "consumable/field-bread");
            assertEquals(5, runtime.invokeObject(bread, "query_restore_amount"));
            assertEquals(12, runtime.invokeObject(bread, "query_travel_restore"));
            Object concord = runtime.invokeObject(catalog, "create", "weapon/concord-focus");
            assertEquals(1, runtime.invokeObject(concord, "query_concord"));
            assertEquals(7, runtime.invokeObject(concord, "query_power"));
            Object badge = runtime.invokeObject(catalog, "create", "badge/open-roof-pin");
            assertEquals("badge", runtime.invokeObject(badge, "query_slot"));
            assertEquals(0, runtime.invokeObject(badge, "query_weight"));
            assertEquals(0, runtime.invokeObject(badge, "query_protection"));
            assertEquals(0, runtime.invokeObject(badge, "query_value"));
            for (String region : List.of("arvenna", "ordel", "talvaren", "caereth", "durnath")) {
                List<?> stock = assertInstanceOf(List.class, runtime.invokeObject(catalog, "stock", "", region));
                assertTrue(stock.size() >= 8);
                assertTrue(ids.containsAll(stock));
                assertTrue(stock.stream().noneMatch(id ->
                        id.toString().startsWith("weapon/concord-") || id.toString().startsWith("badge/")));
            }
            return null;
        });
    }

    @Test
    void exposesTwoStylesAndThreeFiniteTechniquesPerClassInPlayerOrder() {
        world.administer(runtime -> {
            Object rules = runtime.loadOrGetObject("system/combat_rules");
            Map<String, List<String>> styles = Map.of(
                    "fighter", List.of("guard", "assault"),
                    "ranger", List.of("scout", "hunter"),
                    "mage", List.of("ward", "channel"),
                    "cleric", List.of("mercy", "zeal"));
            Map<String, List<String>> techniques = Map.of(
                    "fighter", List.of("mighty-blow", "shield-wall", "pommel-strike"),
                    "ranger", List.of("aimed-shot", "hamstring", "field-dressing"),
                    "mage", List.of("arcane-bolt", "arcane-ward", "unravel"),
                    "cleric", List.of("smite", "mend", "sanctuary"));
            for (String characterClass : styles.keySet()) {
                assertEquals(styles.get(characterClass), runtime.invokeObject(rules, "styles", characterClass));
                assertEquals(techniques.get(characterClass), runtime.invokeObject(rules, "techniques", characterClass));
                int expectedLevel = 1;
                for (String id : techniques.get(characterClass)) {
                    Map<?, ?> profile = assertInstanceOf(Map.class, runtime.invokeObject(
                            rules, "technique", characterClass, id));
                    assertEquals(expectedLevel, profile.get("level"));
                    expectedLevel = expectedLevel == 1 ? 3 : 6;
                }
            }
            return null;
        });
    }
}
