package io.github.protasm.avelorn;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

/** An isolated mudlib root with tracked runtime inputs and an empty account store. */
final class AvelornFixture {
    static final String CONFIG_PATH = "jvmud/avelorn.config";
    private static final String PHYSICAL_ATLAS =
            "atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas";

    private AvelornFixture() {}

    static Path copyTo(Path target) throws IOException {
        Path project = Path.of("").toAbsolutePath();
        for (String directory : List.of("jvmud", "source", PHYSICAL_ATLAS, "atlas/political")) {
            copyTree(project.resolve(directory), target.resolve(directory));
        }
        return target;
    }

    /** Lets atlas-specific tests enter undeveloped terrain with the same movement runtime. */
    static Path copyTerrainFixture(Path target) throws IOException {
        copyTo(target);
        Files.writeString(target.resolve("source/area/registry.c"), """
                mixed *areas() { return ({ }); }
                mapping regions() { return ([ ]); }
                string starting_place() { return "place/atlas/x0113/y3336"; }
                """);
        return target;
    }

    private static void copyTree(Path source, Path target) throws IOException {
        try (var paths = Files.walk(source)) {
            var entries = paths.iterator();
            while (entries.hasNext()) {
                Path entry = entries.next();
                Path destination = target.resolve(source.relativize(entry));
                if (Files.isDirectory(entry)) {
                    Files.createDirectories(destination);
                } else {
                    Files.copy(entry, destination);
                }
            }
        }
    }
}
