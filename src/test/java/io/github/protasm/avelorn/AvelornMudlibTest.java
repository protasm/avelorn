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
import java.util.ArrayList;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

/** Acceptance coverage for the systems preserved across Avelorn's world reset. */
class AvelornMudlibTest {
    @TempDir
    Path tempDir;

    private MudInstance mud;

    @AfterEach
    void stopMud() {
        if (mud != null) {
            mud.shutdown("Test finished.");
        }
    }

    @Test
    void bootsAtTheWesternmostNorthIslandTipAndWalksTheAvailableRoute() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstanceSession persona = mud.attachSession(writer, "127.0.0.1");

        assertEquals("avelorn", mud.gameId());
        assertEquals("place/login", mud.bootResult().initialPlacePath());

        createCharacter(mud, persona, writer, "atlas_walker", "Arden Vale", "male", "ranger");
        assertEquals("place/atlas/x0113/y3336", placeOf(persona));
        commands(mud, persona, writer, "north");
        assertEquals("place/atlas/x0113/y3337", placeOf(persona));
        commands(mud, persona, writer, "south", "east", "west",
                "quests", "inventory", "equipment", "money", "help");

        String transcript = output.toString();
        assertTrue(transcript.contains("The Wild"), transcript);
        assertFalse(transcript.contains("Terrain:"), transcript);
        assertFalse(transcript.contains("Elevation:"), transcript);
        assertTrue(transcript.contains(
                "Wind-rippled dunes rise behind the sandy shore."), transcript);
        assertTrue(transcript.contains("Occupants: none"), transcript);
        assertTrue(transcript.contains("Items: none"), transcript);
        assertTrue(transcript.contains("Exits: n e"), transcript);
        assertTrue(transcript.contains("Your journal contains no assignments."), transcript);
        assertTrue(transcript.contains("ashwood shortbow (equipped)"), transcript);
        assertTrue(transcript.contains("body: blue wool travel cloak (equipped)"), transcript);
        assertTrue(transcript.contains("You carry 1 gold, 2 silver."), transcript);
    }

    @Test
    void wildernessDescriptionsSeparateProseOccupantsAndItemsByMode() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);

        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstanceSession first = mud.attachSession(firstWriter, "127.0.0.1");
        createCharacter(mud, first, firstWriter, "room_first", "Arden Vale", "male", "ranger");

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstanceSession second = mud.attachSession(secondWriter, "127.0.0.2");
        createCharacter(mud, second, secondWriter, "room_second", "Mira Venn", "female", "mage");
        commands(mud, second, secondWriter, "drop draught");
        secondOutput.getBuffer().setLength(0);

        commands(mud, second, secondWriter, "look");
        String verbose = secondOutput.toString();
        assertTrue(verbose.contains("The Wild\n"), verbose);
        assertFalse(verbose.contains("The Wild ("), verbose);
        assertFalse(verbose.contains("Terrain:"), verbose);
        assertFalse(verbose.contains("Elevation:"), verbose);
        assertTrue(verbose.contains(
                "Wind-rippled dunes rise behind the sandy shore."), verbose);
        assertTrue(verbose.contains("Occupants: Arden vale"), verbose);
        assertTrue(verbose.contains("Items: minor healing draught"), verbose);
        assertTrue(verbose.contains("Exits: n e"), verbose);

        secondOutput.getBuffer().setLength(0);
        commands(mud, second, secondWriter, "brief", "north", "south");
        String brief = secondOutput.toString();
        assertTrue(brief.contains("The Wild (113, 3337)"), brief);
        assertTrue(brief.contains("The Wild (113, 3336)"), brief);
        assertTrue(brief.contains("Terrain: Dune Grassland"), brief);
        assertTrue(brief.contains("Elevation: 0 m"), brief);
        assertFalse(brief.contains("terrain stretches through this part of the wilderness."), brief);
        assertTrue(brief.contains("Occupants: Arden vale"), brief);
        assertTrue(brief.contains("Items: minor healing draught"), brief);
        assertTrue(brief.contains("Exits: e s"), brief);
        assertTrue(brief.contains("Exits: n e"), brief);
    }

    @Test
    void publishesCharacterAndNeutralRoomStateOverGmcp() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstanceSession persona = mud.attachSession(writer, "127.0.0.1");
        ArrayList<String> messages = new ArrayList<>();
        mud.bindClientProtocolSink(persona, (protocol, message) -> messages.add(protocol + ":" + message));
        mud.setClientProtocolEnabled(persona, "GMCP", true);

        createCharacter(mud, persona, writer, "gmcp_player", "Mira Venn", "female", "mage");

        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Char.Name ")
                && message.contains("\"name\":\"Mira venn\"")), messages.toString());
        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Char.Base ")
                && message.contains("\"race\":\"human\"")), messages.toString());
        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Char.Vitals ")
                && message.contains("\"hp\":") && message.contains("\"maxhp\":")), messages.toString());
        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Room.Info ")
                && message.contains("\"num\":17080434")
                && message.contains("\"name\":\"The Wild\"")
                && message.contains("\"north\":")
                && message.contains("\"east\":")), messages.toString());
    }

    @Test
    void persistsInventoryAndWildernessPosition() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstanceSession first = mud.attachSession(firstWriter, "127.0.0.1");

        createCharacter(mud, first, firstWriter, "persistence", "Arden Vale", "male", "ranger");
        commands(mud, first, firstWriter, "north", "save", "quit");

        Path snapshotPath = mudlib.resolve("accounts/persistence.1.o");
        assertTrue(Files.isRegularFile(snapshotPath));
        String snapshot = Files.readString(snapshotPath);
        assertTrue(snapshot.contains("inventory_state"), snapshot);
        assertTrue(snapshot.contains("place/atlas/x0113/y3337"), snapshot);
        assertFalse(snapshot.contains("Avelorn1!"), snapshot);

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstanceSession second = mud.attachSession(secondWriter, "127.0.0.2");
        commands(mud, second, secondWriter, "persistence", "Avelorn1!", "score", "inventory", "quests");

        String restored = secondOutput.toString();
        assertTrue(restored.contains("Welcome back, Arden vale."), restored);
        assertTrue(restored.contains("The Wild"), restored);
        assertTrue(restored.contains("level 1 ranger"), restored);
        assertTrue(restored.contains("ashwood shortbow (equipped)"), restored);
        assertTrue(restored.contains("Your journal contains no assignments."), restored);
    }

    @Test
    void derivesWaterCliffAndElevationWalkingConstraintsFromAtlasData() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        var atlas = mud.reloadMudlibObject("system/atlas");

        assertEquals(0, atlas.invoke("walking_result", 2080, 3872, 2081, 3872));
        assertEquals(2, atlas.invoke("walking_result", 766, 3688, 767, 3688));
        assertEquals(3, atlas.invoke("walking_result", 2844, 2557, 2845, 2557));
        assertEquals(4, atlas.invoke("walking_result", 505, 3427, 506, 3427));
        assertEquals(1, atlas.invoke("walking_result", 2080, 3872, 2082, 3872));
    }

    @Test
    void carriesFrozenCatchmentDataIntoRiverScaleDescriptions() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        var atlas = mud.reloadMudlibObject("system/atlas");
        var room = mud.reloadMudlibObject("system/atlas_room");

        assertEquals(7, atlas.invoke("upstream_catchment_ha", 2080, 3872));
        assertEquals(691857, atlas.invoke("upstream_catchment_ha", 4710, 562));

        room.invoke("configure", 4710, 561, 78, 3, 7);
        assertEquals(7, room.invoke("query_upstream_catchment_ha"));
        assertEquals("A narrow headwater stream threads through the low country.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4698, 569, 85, 3, 1094);
        assertEquals("A gathering stream gathers its flow through the low country.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4523, 691, 452, 4, 11973);
        assertEquals("A broad river spreads through waterlogged ground.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4710, 562, 92, 3, 691857);
        assertEquals("A great river carries distant waters through the low country.",
                room.invoke("catchment_description"));
    }

    @Test
    void authorsEveryAvailableTerrainAndUsesOnlyVisibleCatchment() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        var room = mud.reloadMudlibObject("system/atlas_room");

        for (int terrain = 1; terrain <= 62; terrain++) {
            room.invoke("configure", 1000 + terrain, 2000, 450, terrain, 12000);
            String description = (String) room.invoke("wilderness_description");
            assertFalse(description.isBlank(), "terrain " + terrain);
            assertTrue(description.endsWith("."), description);
            assertFalse(description.contains(" terrain "), description);
            assertFalse(description.contains("terrain-code-"), description);
        }

        room.invoke("configure", 2080, 3872, 309, 23, 7);
        String dryCatchment = (String) room.invoke("wilderness_description");
        room.invoke("configure", 2080, 3872, 309, 23, 691857);
        assertEquals(dryCatchment, room.invoke("wilderness_description"));

        room.invoke("configure", 2080, 3872, 30, 23, 7);
        String low = (String) room.invoke("wilderness_description");
        room.invoke("configure", 2080, 3872, 900, 23, 7);
        String high = (String) room.invoke("wilderness_description");
        assertEquals("Wooded foothills roll through the low-lying country.", low);
        assertEquals("Wooded foothills roll through the high country.", high);

        room.invoke("configure", 2080, 3872, 30, 15, 7);
        low = (String) room.invoke("wilderness_description");
        room.invoke("configure", 2080, 3872, 900, 15, 7);
        high = (String) room.invoke("wilderness_description");
        assertTrue(low.contains("low-lying country"), low);
        assertTrue(high.contains("high country"), high);

        room.invoke("configure", 2080, 3872, 309, 15, 7);
        String firstVariant = (String) room.invoke("wilderness_description");
        assertEquals(firstVariant, room.invoke("wilderness_description"));
        room.invoke("configure", 2081, 3872, 309, 15, 7);
        assertFalse(firstVariant.equals(room.invoke("wilderness_description")));
    }

    @Test
    void addsOneNotableAdjacentFeatureOnlyToVerboseRoomProse() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        var room = mud.reloadMudlibObject("system/atlas_room");

        room.invoke("configure", 2080, 3872, 309, 23, 7);
        assertEquals("", room.invoke("adjacent_description"));

        room.invoke("configure", 767, 3689, 89, 10, 7);
        assertEquals("A river lies to the south.", room.invoke("adjacent_description"));

        room.invoke("configure", 2844, 2557, 1076, 32, 20);
        assertEquals("A precipice looms to the east.", room.invoke("adjacent_description"));

        room.invoke("configure", 505, 3427, 430, 40, 7);
        assertEquals("The land falls sharply to the east.", room.invoke("adjacent_description"));

        room.invoke("configure", 2057, 4904, 3, 55, 13);
        assertEquals("Dune Grassland begins to the north.",
                room.invoke("adjacent_description"));
    }

    @Test
    void retainsCommunicationAndOneHundredSocialEmotes() throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstanceSession first = mud.attachSession(firstWriter, "127.0.0.1");
        createCharacter(mud, first, firstWriter, "speaker", "Linnet Grey", "female", "ranger");

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstanceSession second = mud.attachSession(secondWriter, "127.0.0.2");
        createCharacter(mud, second, secondWriter, "listener", "Arden Vale", "male", "cleric");

        commands(mud, first, firstWriter, "emotes", "grin", "wave", "say Hello",
                "tell arden vale Quiet hello");

        assertTrue(firstOutput.toString().contains("Avelorn emotes (100):"), firstOutput.toString());
        assertTrue(firstOutput.toString().contains("You grin."), firstOutput.toString());
        assertTrue(secondOutput.toString().contains("Linnet grey waves."), secondOutput.toString());
        assertTrue(secondOutput.toString().contains("Linnet grey says, \"Hello\""), secondOutput.toString());
        assertTrue(secondOutput.toString().contains("Linnet grey tells you, \"Quiet hello\""), secondOutput.toString());
    }

    @ParameterizedTest
    @CsvSource({
        "fighter, arming sword, Strength 14, Stamina",
        "ranger, ashwood shortbow, Dexterity 14, Stamina",
        "mage, oak focus staff, Intelligence 14, Mana",
        "cleric, steel mace, Wisdom 14, Faith"
    })
    void retainsEveryClassAndStarterKit(
            String characterClass,
            String weapon,
            String primaryAttribute,
            String resourceName) throws Exception {
        Path mudlib = AvelornFixture.copyTerrainFixture(tempDir.resolve("avelorn"));
        mud = MudInstance.boot(mudlib, AvelornFixture.CONFIG_PATH);
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstanceSession persona = mud.attachSession(writer, "127.0.0.1");

        createCharacter(mud, persona, writer, "class_" + characterClass, "Celyn Ward", "female", characterClass);
        commands(mud, persona, writer, "inventory", "score");

        String transcript = output.toString();
        assertTrue(transcript.contains("level 1 " + characterClass), transcript);
        assertTrue(transcript.contains(weapon + " (equipped)"), transcript);
        assertTrue(transcript.contains(primaryAttribute), transcript);
        assertTrue(transcript.contains(resourceName + " "), transcript);
    }

    private String placeOf(InstanceSession session) {
        return mud.administer(runtime -> (String) runtime.invokeObject(
                runtime.loadOrGetObject("system/world"), "place_path",
                runtime.environment(session.personaObject())));
    }

    private void createCharacter(
            MudInstance mud,
            InstanceSession persona,
            PrintWriter writer,
            String account,
            String name,
            String gender,
            String characterClass) {
        commands(mud, persona, writer, account, "yes", "Avelorn1!", "Avelorn1!", name, gender, characterClass);
    }

    private void commands(
            MudInstance mud,
            InstanceSession persona,
            PrintWriter writer,
            String... commands) {
        for (String command : commands) {
            mud.dispatch(persona, writer, command);
            mud.advanceWorldTick();
        }
    }
}
