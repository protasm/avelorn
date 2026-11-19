package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Path;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.EnumSource;
import org.junit.jupiter.params.provider.ValueSource;

/** Drives the authored campaign through the same commands available in a Telnet session. */
class AvelornCampaignTest {
    private static final String PASSWORD = "Avelorn1!";
    private static final String GUEST_HOUSE = "place/westwatch/guest-house";
    private static final String LANDING = "place/westwatch/landing";
    private static final String VAULT = "place/deep-vault/seam-guard";
    private static final List<Chapter> CHAPTERS = List.of(
            new Chapter("Havenrode", "havenrode", "salt-ledger", "west", "east",
                    "the-guests-roll", "The Guests' Roll", "harl", "Bond Broker Harl"),
            new Chapter("Witnessgate", "witnessgate", "record-vault", "west", "east",
                    "a-witness-for-the-lost", "A Witness for the Lost", "vey", "Deputy Vey"),
            new Chapter("Keepingstead", "keepingstead", "silt-house", "west", "east",
                    "a-storehouse-in-trust", "A Storehouse in Trust", "fenn", "Ration Sergeant Fenn"),
            new Chapter("Freeharbor", "freeharbor", "tide-chapel", "west", "east",
                    "the-unforced-oath", "The Unforced Oath", "carn", "Bailiff Carn"),
            new Chapter("Ironhold", "ironhold", "empty-winch", "east", "west",
                    "a-rope-held-fast", "A Rope Held Fast", "carrow", "Overseer Carrow"));

    @TempDir
    Path tempDir;

    private MudInstance world;

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Campaign test complete.");
            world = null;
        }
    }

    @Test
    void opensInWestwatchAndRestoresTheCompletedQuestAfterRestart() throws Exception {
        Path mudlib = AvelornFixture.copyTo(tempDir.resolve("westwatch"));
        boot(mudlib);
        Client traveler = create("westwatch_guest", Archetype.RANGER);
        assertText(command(traveler, "look lamp"), "Two panes of blue glass");
        completeHeadland(List.of(traveler));
        assertText(command(traveler, "inventory"), "loaf of field bread");
        String money = command(traveler, "money");
        assertText(money, "You carry");
        command(traveler, "save");
        world.detachSession(traveler.session());
        stopWorld();

        boot(mudlib);
        Client returning = connect(Archetype.RANGER);
        commands(returning, "westwatch_guest", PASSWORD);
        assertText(returning.read(), "Welcome back");
        assertEquals(GUEST_HOUSE, room(returning));
        assertText(command(returning, "journal"), "Lamp on the Headland — complete");
        assertText(command(returning, "inventory"), "loaf of field bread");
        assertEquals(money, command(returning, "money"));

        String duplicate = command(returning, "report lamp-on-the-headland");
        assertText(duplicate, "already");
        assertFalse(duplicate.contains("You receive"), duplicate);
        assertEquals(money, command(returning, "money"));
    }

    @ParameterizedTest(name = "{0} can finish the campaign solo")
    @EnumSource(Archetype.class)
    void playsTheEightChaptersSolo(Archetype archetype) throws Exception {
        Path mudlib = AvelornFixture.copyTo(tempDir.resolve(archetype.id));
        boot(mudlib);
        shortenTransit();
        Client traveler = create("traveler_" + archetype.id, archetype);
        completeCampaign(List.of(traveler));
    }

    @ParameterizedTest(name = "{0} players can finish the campaign together")
    @ValueSource(ints = {2, 4})
    void playsTheEightChaptersTogetherWithSharedVictoryCredit(int playerCount) throws Exception {
        Path mudlib = AvelornFixture.copyTo(tempDir.resolve("party_campaign"));
        boot(mudlib);
        shortenTransit();
        Client fighter = create("party_fighter", Archetype.FIGHTER, "Alder Vale");
        List<Client> travelers = new ArrayList<>();
        travelers.add(fighter);
        travelers.add(recruit(fighter, Archetype.CLERIC, "Linnet Grey"));
        if (playerCount == 4) {
            travelers.add(recruit(fighter, Archetype.RANGER, "Vessa Reed"));
            travelers.add(recruit(fighter, Archetype.MAGE, "Kara Stone"));
        }
        assertText(command(fighter, "party"), "(" + playerCount + "/4)");
        completeCampaign(travelers);
    }

    private Client recruit(Client leader, Archetype archetype, String name) {
        Client member = create("party_" + archetype.id, archetype, name);
        assertText(command(leader, "party invite " + name), "You invite ");
        assertText(command(member, "party accept alder vale"), "joins the party.");
        return member;
    }

    private void completeCampaign(List<Client> travelers) throws InterruptedException {
        for (Client traveler : travelers) {
            assertText(command(traveler, "equipment"), traveler.archetype().starterWeapon);
        }
        completeHeadland(travelers);
        move(travelers, "south", LANDING);

        for (Chapter chapter : CHAPTERS) {
            board(travelers, chapter.city(), chapter.room("arrival"));
            completeCityChapter(travelers, chapter);
        }

        board(travelers, "Lakeward", "place/lakeward/arrival");
        move(travelers, "out east east east enter north", "place/cinder-bank/forge");
        offer(travelers, "the-concordance", "The Concordance");
        assertGroup(command(travelers, "forge concordance"), "The Concordance: 1/1");
        report(travelers, "the-concordance", "The Concordance");
        for (Client traveler : travelers) {
            assertText(command(traveler, "equip " + traveler.archetype().concordWeapon), "You equip concord");
        }
        move(travelers, "south out west west west enter", "place/lakeward/arrival");

        board(travelers, "Coalstair", "place/coalstair/arrival");
        move(travelers, "north", "place/coalstair/hearth");
        assertGroup(command(travelers, "rest"), "You rest");
        offer(travelers, "under-the-mountain", "Under the Mountain");

        move(travelers, "south out east east east east east east", "place/atlas/x2401/y3726");
        move(travelers, "east east east east east east east east", "place/atlas/x2409/y3726");
        move(travelers, "east east east south south south south south", "place/atlas/x2412/y3721");
        move(travelers, "south enter down", VAULT);
        assertGroup(command(travelers, "consider warden"), "Warden Senk is level 10");
        assertGroup(win(travelers, "warden", "Warden Senk"),
                "Concord metal makes the bonded armor go still.");
        move(travelers, "up out north north north north north north", "place/atlas/x2412/y3726");
        move(travelers, "west west west west west west west west", "place/atlas/x2404/y3726");
        move(travelers, "west west west west west west west west", "place/atlas/x2396/y3726");
        move(travelers, "west enter north", "place/coalstair/hearth");
        report(travelers, "under-the-mountain", "Under the Mountain");

        for (Client traveler : travelers) {
            String journal = command(traveler, "journal");
            assertText(journal, "Under the Mountain — complete");
            assertEquals(8, journal.lines().filter(line -> line.endsWith("— complete")).count(), journal);
            assertText(command(traveler, "score"), "level 10 " + traveler.archetype().id);
        }
    }

    private void completeHeadland(List<Client> travelers) {
        offer(travelers, "lamp-on-the-headland", "Lamp on the Headland");
        move(travelers, "south out east enter", "place/headland-signal/lamp-walk");
        win(travelers, "wolf", "headland wolf");
        move(travelers, "out west enter north", GUEST_HOUSE);
        report(travelers, "lamp-on-the-headland", "Lamp on the Headland");
    }

    private void completeCityChapter(List<Client> travelers, Chapter chapter) {
        move(travelers, "north west", chapter.room("hearth"));
        assertGroup(command(travelers, "rest"), "You rest");
        move(travelers, "east north", chapter.room("hall"));
        offer(travelers, chapter.questId(), chapter.title());

        String outbound = chapter.outbound();
        move(travelers, "south south out " + outbound + " " + outbound + " " + outbound + " enter north",
                "place/" + chapter.site() + "/works");
        win(travelers, chapter.enemyAlias(), chapter.enemyName());
        String returning = chapter.returning();
        move(travelers, "south out " + returning + " " + returning + " " + returning + " enter north north",
                chapter.room("hall"));
        report(travelers, chapter.questId(), chapter.title());
        move(travelers, "south south", chapter.room("arrival"));
    }

    private void offer(List<Client> travelers, String questId, String title) {
        assertGroup(command(travelers, "work " + questId), title + " (recommended level");
    }

    private void report(List<Client> travelers, String questId, String title) {
        assertGroup(command(travelers, "report " + questId), "You complete " + title + ".");
    }

    private List<String> win(List<Client> travelers, String enemyAlias, String enemyName) {
        List<StringBuilder> combat = new ArrayList<>();
        List<String> techniques = new ArrayList<>();
        for (int index = 0; index < travelers.size(); index++) {
            Client traveler = travelers.get(index);
            if (index == 1 && enemyAlias.equals("carn")) {
                // The cleric earns this victory by guarding the fighter, who is already engaging Carn.
                send(traveler, "ability sanctuary alder vale");
                techniques.add("Sanctuary");
            } else {
                Archetype archetype = traveler.archetype();
                send(traveler, "ability " + archetype.techniqueId + " " + enemyAlias);
                techniques.add(archetype.techniqueName);
            }
        }
        world.advanceWorldTick();
        for (int index = 0; index < travelers.size(); index++) {
            String opening = travelers.get(index).read();
            assertText(opening, "You use " + techniques.get(index) + ".");
            combat.add(new StringBuilder(opening));
        }
        String expected = enemyName + " is defeated.";
        for (int tick = 0; tick < 24 && combat.stream().noneMatch(log -> log.toString().contains(expected)); tick++) {
            world.advanceWorldTick();
            for (int index = 0; index < travelers.size(); index++) {
                combat.get(index).append(travelers.get(index).read());
            }
        }
        if (travelers.size() > 1 && enemyAlias.equals("carn")) {
            assertText(combat.getFirst().toString(), "You are braced for the next strike.");
        }
        for (StringBuilder log : combat) {
            assertText(log.toString(), expected);
            assertText(log.toString(), "You help defeat " + enemyName + ".");
            assertFalse(log.toString().contains("You are overcome"), log::toString);
        }
        return combat.stream().map(StringBuilder::toString).toList();
    }

    private void move(List<Client> travelers, String directions, String target) {
        StringBuilder transcript = new StringBuilder(String.join("", command(travelers, "walk " + directions)));
        for (int tick = 0; tick < 45 && !allAt(travelers, target); tick++) {
            world.advanceWorldTick();
            travelers.forEach(traveler -> transcript.append(traveler.read()));
        }
        assertTrue(allAt(travelers, target), () -> directions + "\n" + transcript);
    }

    private void board(List<Client> travelers, String destination, String target) throws InterruptedException {
        assertGroup(command(travelers, "routes"), "to " + destination);
        assertGroup(command(travelers, "board " + destination), "You board");
        long deadline = System.nanoTime() + Duration.ofSeconds(3).toNanos();
        StringBuilder transit = new StringBuilder();
        while (!allAt(travelers, target) && System.nanoTime() < deadline) {
            Thread.sleep(15);
            world.advanceWorldTick();
            travelers.forEach(traveler -> transit.append(traveler.read()));
        }
        assertTrue(allAt(travelers, target), transit::toString);
    }

    private boolean allAt(List<Client> travelers, String path) {
        return travelers.stream().allMatch(traveler -> path.equals(room(traveler)));
    }

    private List<String> command(List<Client> travelers, String line) {
        travelers.forEach(traveler -> send(traveler, line));
        world.advanceWorldTick();
        return travelers.stream().map(Client::read).toList();
    }

    private static void assertGroup(List<String> transcripts, String expected) {
        transcripts.forEach(transcript -> assertText(transcript, expected));
    }

    @SuppressWarnings("unchecked")
    private void shortenTransit() {
        world.administer(runtime -> {
            Object catalog = runtime.loadOrGetObject("system/world");
            var routes = (Map<String, Map<String, Object>>) runtime.invokeObject(catalog, "section", "routes");
            assertFalse(routes.isEmpty());
            routes.values().forEach(route -> route.put("duration", 1));
            return null;
        });
    }

    private void boot(Path root) {
        world = MudInstance.boot(root, AvelornFixture.CONFIG_PATH);
    }

    private Client create(String account, Archetype archetype) {
        return create(account, archetype, "Alder Vale");
    }

    private Client create(String account, Archetype archetype, String name) {
        Client client = connect(archetype);
        commands(client, account, "yes", PASSWORD, PASSWORD, name, "female", archetype.id);
        String welcome = client.read();
        assertText(welcome, "Westwatch Guest-House");
        assertText(welcome, "Iona Venn");
        assertText(welcome, "blue lamp");
        assertFalse(welcome.contains("[lamp]"), welcome);
        command(client, "brief");
        return client;
    }

    private Client connect(Archetype archetype) {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        return new Client(world.attachSession(writer, "127.0.0.1"), writer, output, archetype);
    }

    private void commands(Client client, String... lines) {
        for (String line : lines) {
            send(client, line);
            world.advanceWorldTick();
        }
    }

    private void send(Client client, String line) {
        world.dispatch(client.session(), client.writer(), line);
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

    private static void assertText(String transcript, String expected) {
        assertTrue(transcript.contains(expected), () -> "Expected: " + expected + "\n" + transcript);
    }

    private record Chapter(String city, String cityId, String site, String outbound, String returning,
                           String questId, String title, String enemyAlias, String enemyName) {
        String room(String local) {
            return "place/" + cityId + "/" + local;
        }
    }

    private enum Archetype {
        FIGHTER("fighter", "arming sword", "mighty-blow", "Mighty Blow", "concord-sword"),
        RANGER("ranger", "ashwood shortbow", "aimed-shot", "Aimed Shot", "concord-bow"),
        MAGE("mage", "oak focus staff", "arcane-bolt", "Arcane Bolt", "concord-focus"),
        CLERIC("cleric", "steel mace", "smite", "Smite", "concord-mace");

        private final String id;
        private final String starterWeapon;
        private final String techniqueId;
        private final String techniqueName;
        private final String concordWeapon;

        Archetype(String id, String starterWeapon, String techniqueId, String techniqueName, String concordWeapon) {
            this.id = id;
            this.starterWeapon = starterWeapon;
            this.techniqueId = techniqueId;
            this.techniqueName = techniqueName;
            this.concordWeapon = concordWeapon;
        }
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output, Archetype archetype) {
        String read() {
            String transcript = output.toString();
            output.getBuffer().setLength(0);
            return transcript;
        }
    }
}
