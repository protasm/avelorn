package io.github.protasm.jvmud.instance;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

/** Acceptance coverage for the systems preserved across Avelorn's world reset. */
class AvelornMudlibTest {
    @TempDir
    Path tempDir;

    @Test
    void bootsIntoTheEditionOneAtlasAndWalksCardinally() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstancePersona persona = mud.attachPersona(writer, "127.0.0.1");

        assertEquals("avelorn", mud.gameId());
        assertEquals("place/atlas/x2080/y3872", mud.startingPlacePath());

        createCharacter(mud, persona, writer, "atlas_walker", "Arden Vale", "non-binary", "ranger");
        commands(mud, persona, writer, "north", "south", "east", "west",
                "quests", "inventory", "equipment", "money", "help");

        String transcript = output.toString();
        assertTrue(transcript.contains("The Wild"), transcript);
        assertFalse(transcript.contains("Terrain:"), transcript);
        assertFalse(transcript.contains("Elevation:"), transcript);
        assertTrue(transcript.contains(
                "Forested Foothill terrain stretches through this part of the wilderness."), transcript);
        assertTrue(transcript.contains("Occupants: none"), transcript);
        assertTrue(transcript.contains("Items: none"), transcript);
        assertTrue(transcript.contains("Exits: n e s w"), transcript);
        assertTrue(transcript.contains("Your journal contains no assignments."), transcript);
        assertTrue(transcript.contains("ashwood shortbow (equipped)"), transcript);
        assertTrue(transcript.contains("body: blue wool travel cloak (equipped)"), transcript);
        assertTrue(transcript.contains("You carry 1 gold, 2 silver."), transcript);
    }

    @Test
    void wildernessDescriptionsSeparateProseOccupantsAndItemsByMode() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");

        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstancePersona first = mud.attachPersona(firstWriter, "127.0.0.1");
        createCharacter(mud, first, firstWriter, "room_first", "Arden Vale", "non-binary", "ranger");

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstancePersona second = mud.attachPersona(secondWriter, "127.0.0.2");
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
                "Forested Foothill terrain stretches through this part of the wilderness."), verbose);
        assertTrue(verbose.contains("Occupants: Arden vale"), verbose);
        assertTrue(verbose.contains("Items: minor healing draught"), verbose);
        assertTrue(verbose.contains("Exits: n e s w"), verbose);

        secondOutput.getBuffer().setLength(0);
        commands(mud, second, secondWriter, "brief", "north", "south");
        String brief = secondOutput.toString();
        assertTrue(brief.contains("The Wild (2080, 3873)"), brief);
        assertTrue(brief.contains("The Wild (2080, 3872)"), brief);
        assertTrue(brief.contains("Terrain: Forested Foothill"), brief);
        assertTrue(brief.contains("Elevation: 309 m"), brief);
        assertFalse(brief.contains("terrain stretches through this part of the wilderness."), brief);
        assertTrue(brief.contains("Occupants: Arden vale"), brief);
        assertTrue(brief.contains("Items: minor healing draught"), brief);
        assertTrue(brief.contains("Exits: n e s w"), brief);
    }

    @Test
    void publishesCharacterAndNeutralRoomStateOverGmcp() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstancePersona persona = mud.attachPersona(writer, "127.0.0.1");
        ArrayList<String> messages = new ArrayList<>();
        mud.bindClientProtocolSink(persona, (protocol, message) -> messages.add(protocol + ":" + message));
        mud.setClientProtocolEnabled(persona, "GMCP", true);

        createCharacter(mud, persona, writer, "gmcp_player", "Mira Venn", "female", "mage");

        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Char.Name ")
                && message.contains("\"name\":\"Mira venn\"")), messages.toString());
        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Char.Vitals ")
                && message.contains("\"hp\":") && message.contains("\"maxhp\":")), messages.toString());
        assertTrue(messages.stream().anyMatch(message -> message.startsWith("GMCP:Room.Info ")
                && message.contains("\"num\":19826721")
                && message.contains("\"name\":\"The Wild\"")
                && message.contains("\"north\":")
                && message.contains("\"east\":")
                && message.contains("\"south\":")
                && message.contains("\"west\":")), messages.toString());
    }

    @Test
    void persistsCharacterSystemsWhileDiscardingOldWorldAndQuestState() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstancePersona first = mud.attachPersona(firstWriter, "127.0.0.1");

        createCharacter(mud, first, firstWriter, "persistence", "Arden Vale", "non-binary", "ranger");
        commands(mud, first, firstWriter, "north", "save", "quit");

        Path snapshotPath = mudlib.resolve("accounts/persistence.o");
        assertTrue(Files.isRegularFile(snapshotPath));
        String snapshot = Files.readString(snapshotPath);
        assertTrue(snapshot.contains("inventory_state"), snapshot);
        assertTrue(snapshot.contains("place/atlas/x2080/y3873"), snapshot);
        assertFalse(snapshot.contains("Avelorn1!"), snapshot);

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstancePersona second = mud.attachPersona(secondWriter, "127.0.0.2");
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
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        var atlas = mud.reloadMudlibObject("system/atlas");

        assertEquals(0, atlas.invoke("walking_result", 2080, 3872, 2081, 3872));
        assertEquals(2, atlas.invoke("walking_result", 766, 3688, 767, 3688));
        assertEquals(3, atlas.invoke("walking_result", 2844, 2557, 2845, 2557));
        assertEquals(4, atlas.invoke("walking_result", 505, 3427, 506, 3427));
        assertEquals(1, atlas.invoke("walking_result", 2080, 3872, 2082, 3872));
    }

    @Test
    void carriesFrozenCatchmentDataIntoRiverScaleDescriptions() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        var atlas = mud.reloadMudlibObject("system/atlas");
        var room = mud.reloadMudlibObject("system/atlas_room");

        assertEquals(7, atlas.invoke("upstream_catchment_ha", 2080, 3872));
        assertEquals(691857, atlas.invoke("upstream_catchment_ha", 4710, 562));

        room.invoke("configure", 4710, 561, 78, 3, 7);
        assertEquals(7, room.invoke("query_upstream_catchment_ha"));
        assertEquals("A headwater stream drains the nearby slopes.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4698, 569, 85, 3, 1094);
        assertEquals("A stream gathers the local drainage.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4523, 691, 452, 4, 11973);
        assertEquals("A river drains the surrounding country.",
                room.invoke("catchment_description"));

        room.invoke("configure", 4710, 562, 92, 3, 691857);
        assertEquals("A major river drains a vast watershed.",
                room.invoke("catchment_description"));
    }

    @Test
    void addsOneNotableAdjacentFeatureOnlyToVerboseRoomProse() throws Exception {
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
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
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        StringWriter firstOutput = new StringWriter();
        PrintWriter firstWriter = new PrintWriter(firstOutput, true);
        InstancePersona first = mud.attachPersona(firstWriter, "127.0.0.1");
        createCharacter(mud, first, firstWriter, "speaker", "Linnet Grey", "female", "ranger");

        StringWriter secondOutput = new StringWriter();
        PrintWriter secondWriter = new PrintWriter(secondOutput, true);
        InstancePersona second = mud.attachPersona(secondWriter, "127.0.0.2");
        createCharacter(mud, second, secondWriter, "listener", "Arden Vale", "non-binary", "cleric");

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
        Path mudlib = copyAvelornFixture();
        MudInstance mud = MudInstance.boot(mudlib, "jvmud/avelorn.config");
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        InstancePersona persona = mud.attachPersona(writer, "127.0.0.1");

        createCharacter(mud, persona, writer, "class_" + characterClass, "Celyn Ward", "female", characterClass);
        commands(mud, persona, writer, "inventory", "score");

        String transcript = output.toString();
        assertTrue(transcript.contains("level 1 " + characterClass), transcript);
        assertTrue(transcript.contains(weapon + " (equipped)"), transcript);
        assertTrue(transcript.contains(primaryAttribute), transcript);
        assertTrue(transcript.contains(resourceName + " "), transcript);
    }

    private void createCharacter(
            MudInstance mud,
            InstancePersona persona,
            PrintWriter writer,
            String account,
            String name,
            String gender,
            String characterClass) {
        commands(mud, persona, writer, account, "yes", "Avelorn1!", "Avelorn1!", name, gender, characterClass);
    }

    private void commands(
            MudInstance mud,
            InstancePersona persona,
            PrintWriter writer,
            String... commands) {
        for (String command : commands) {
            mud.dispatch(persona, writer, command);
        }
    }

    private Path copyAvelornFixture() throws IOException {
        Path source = Path.of(".").toAbsolutePath().normalize();
        Path target = tempDir.resolve("avelorn");
        try (var paths = Files.walk(source)) {
            paths.filter(path -> !excluded(source, path)).forEach(path -> copyPath(source, target, path));
        }
        return target;
    }

    private boolean excluded(Path source, Path path) {
        Path relative = source.relativize(path);
        return relative.startsWith(".git")
                || relative.startsWith("target")
                || (relative.startsWith("accounts") && path.getFileName().toString().endsWith(".o"));
    }

    private void copyPath(Path source, Path target, Path path) {
        try {
            Path destination = target.resolve(source.relativize(path));
            if (Files.isDirectory(path)) {
                Files.createDirectories(destination);
            } else {
                Files.copy(path, destination);
            }
        } catch (IOException e) {
            throw new IllegalStateException("Could not copy Avelorn fixture", e);
        }
    }
}
