#!/bin/zsh
# Rebuild this derivative without replacing an existing Minecraft save.
set -eu
task_root="${0:A:h:h:h:h}"
task_python="${AVELORN_PYTHON:-/Users/jonathan/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/bin/python3}"
task_worldpainter="${AVELORN_WORLDPAINTER:-/Users/jonathan/Applications/WorldPainter.app}"
task_classes="$(mktemp -d /tmp/avelorn-minecraft-classes.XXXXXX)"
cd "$task_root"
"$task_python" atlas/minecraft/tools/prepare_north_island.py
javac -cp "$task_worldpainter/Contents/Resources/app/lib/*" -d "$task_classes" atlas/minecraft/tools/BuildNorthIsland.java
java -Xmx6g -Djava.awt.headless=true -cp "$task_classes:$task_worldpainter/Contents/Resources/app/lib/*" BuildNorthIsland output/minecraft
"$task_python" atlas/minecraft/tools/validate_world.py
