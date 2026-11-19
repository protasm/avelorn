package io.github.protasm.avelorn;

import io.github.protasm.jvmud.engine.instance.MudInstance;
import io.github.protasm.jvmud.engine.instance.session.InstanceSession;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.file.Path;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

final class AvelornContentTest {
    private static final String ACCOUNT = "lamp_keeper";
    private static final String PASSWORD = "Avelorn1!";

    @TempDir
    Path temporary;

    @Test
    void verifiesTheCompiledLPCWorldAndEveryAuthoredRoom() throws Exception {
        Path fixture = AvelornFixture.copyTo(temporary.resolve("content"));
        var world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            Map<?, ?> counts = world.administer(runtime -> (Map<?, ?>) runtime.invokeObject(
                    runtime.loadOrGetObject("system/content_check"), "verify"));
            assertEquals(5, counts.get("regions"), counts.toString());
            assertTrue((int) counts.get("cities") >= 5, counts.toString());
            assertTrue((int) counts.get("settlements") >= 15, counts.toString());
            assertTrue((int) counts.get("sites") >= 10, counts.toString());
            assertTrue((int) counts.get("details") >= (int) counts.get("rooms"), counts.toString());
        } finally {
            world.shutdown(0);
        }
    }

    @Test
    void roomHintsStartOffAndCanBeToggledPersistedAndUsedWithBrief() throws Exception {
        Path fixture = AvelornFixture.copyTo(temporary.resolve("hints"));
        var world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            var out = new StringWriter();
            var writer = new PrintWriter(out, true);
            var player = world.attachSession(writer, "127.0.0.1");
            send(world, player, writer, ACCOUNT, "yes", PASSWORD, PASSWORD, "Rian Bell", "male", "ranger");

            assertTrue(out.toString().contains("blue lamp"), out.toString());
            assertFalse(out.toString().contains("[lamp]"), out.toString());
            out.getBuffer().setLength(0);
            send(world, player, writer, "details", "exa lamp", "exa kettle");
            assertTrue(out.toString().contains("Details: lamp, noticeboard."), out.toString());
            assertTrue(out.toString().contains("scratched initials"), out.toString());
            assertTrue(out.toString().contains("You do not see that here."), out.toString());

            out.getBuffer().setLength(0);
            send(world, player, writer, "hints on", "look", "brief", "south", "north");
            assertTrue(out.toString().contains("blue [lamp]"), out.toString());
            assertTrue(out.toString().contains("Details: [lamp], [noticeboard]."), out.toString());
            world.detachSession(player);
        } finally {
            world.shutdown(0);
        }
        world = MudInstance.boot(fixture, AvelornFixture.CONFIG_PATH);
        try {
            var out = new StringWriter();
            var writer = new PrintWriter(out, true);
            var player = world.attachSession(writer, "127.0.0.1");
            send(world, player, writer, ACCOUNT, PASSWORD, "hints");
            assertTrue(out.toString().contains("Inspection hints are on."), out.toString());

            out.getBuffer().setLength(0);
            send(world, player, writer, "hints off", "look", "south", "north");
            assertTrue(out.toString().contains("blue lamp"), out.toString());
            assertTrue(out.toString().contains("Details: lamp, noticeboard."), out.toString());
            assertFalse(out.toString().contains("[lamp]"), out.toString());
        } finally {
            world.shutdown(0);
        }
    }

    private void send(MudInstance world, InstanceSession player, PrintWriter writer, String... commands) {
        for (String command : commands) {
            world.dispatch(player, writer, command);
            world.advanceWorldTick();
        }
    }
}
