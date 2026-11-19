package io.github.protasm.avelorn;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Path;
import java.util.List;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

class AvelornPartyTest {
    @TempDir
    Path tempDir;

    private MudInstance world;

    @AfterEach
    void stopWorld() {
        if (world != null) {
            world.shutdown("Party test complete.");
        }
    }

    @Test
    void partyRequiresConsentAndOnlyOptedInMembersFollow() throws Exception {
        Path root = AvelornFixture.copyTo(tempDir.resolve("avelorn"));
        world = MudInstance.boot(root, AvelornFixture.CONFIG_PATH);
        Client leader = create("party_leader", "Arden Vale");
        Client follower = create("party_follower", "Mira Vale");
        Client companion = create("party_companion", "Neri Vale");

        command(follower, "party accept Arden Vale");
        assertEquals(1, roster(follower).size());
        command(leader, "party invite Mira Vale");
        assertEquals(1, roster(leader).size());
        command(follower, "party decline Arden Vale");
        assertEquals(1, roster(follower).size());

        command(leader, "party invite Mira Vale");
        command(follower, "east");
        command(follower, "party accept Arden Vale");
        assertEquals(1, roster(follower).size());
        command(follower, "west");
        command(follower, "party accept Arden Vale");
        command(leader, "party invite Neri Vale");
        command(companion, "party accept Arden Vale");
        assertEquals(List.of(leader.actor(), follower.actor(), companion.actor()), roster(leader));

        command(follower, "party follow on");
        command(leader, "walk east west south out north");
        for (int tick = 0; tick < 4; tick++) {
            world.advanceWorldTick();
        }
        assertEquals("place/atlas/x0113/y3337", room(leader));
        assertEquals(room(leader), room(follower));
        assertEquals(reserve(leader), reserve(follower));
        assertTrue(reserve(follower) < 100);
        assertEquals("place/westwatch/guest-house", room(companion));
        assertEquals(100, reserve(companion));

        send(follower, "stop");
        command(leader, "south");
        world.advanceWorldTick();
        assertEquals("place/atlas/x0113/y3336", room(leader));
        assertEquals("place/atlas/x0113/y3337", room(follower));

        world.detachSession(leader.session());
        assertEquals(List.of(follower.actor(), companion.actor()), roster(follower));
        command(follower, "party");
        assertTrue(follower.transcript().contains("Party led by Mira vale (2/4)"), follower.transcript());
        command(follower, "party say regroup at the landing");
        assertTrue(companion.transcript().contains("regroup at the landing"), companion.transcript());
        world.detachSession(follower.session());
        assertEquals(List.of(companion.actor()), roster(companion));
    }

    private Client create(String account, String name) {
        StringWriter output = new StringWriter();
        PrintWriter writer = new PrintWriter(output, true);
        Client client = new Client(world.attachSession(writer, "127.0.0.1"), writer, output);
        for (String line : List.of(account, "yes", "Avelorn1!", "Avelorn1!", name, "female", "ranger")) {
            send(client, line);
        }
        assertTrue(client.transcript().contains("Welcome to Avelorn"), client.transcript());
        return client;
    }

    private void command(Client client, String line) {
        send(client, line);
        world.advanceWorldTick();
    }

    private void send(Client client, String line) {
        world.dispatch(client.session(), client.writer(), line);
    }

    private List<?> roster(Client client) {
        return world.administer(runtime -> (List<?>) runtime.invokeObject(
                runtime.loadOrGetObject("system/party"), "roster", client.actor()));
    }

    private String room(Client client) {
        return world.administer(runtime -> (String) runtime.invokeObject(
                runtime.loadOrGetObject("system/world"), "place_path", runtime.environment(client.actor())));
    }

    private int reserve(Client client) {
        return world.administer(runtime -> ((Number) runtime.invokeObject(
                client.actor(), "query_travel_reserve")).intValue());
    }

    private record Client(InstanceSession session, PrintWriter writer, StringWriter output) {
        Object actor() {
            return session.personaObject();
        }

        String transcript() {
            return output.toString();
        }
    }
}
