# Avelorn

Avelorn is a persistent multiplayer game authored for the JVMud engine.

The geographical world and its story have been intentionally reset. All former
regions, rooms, routes, maps, placed inhabitants, shops, quests, and generated
world logic are gone. `source/place/login_room.c` is a temporary login room
required by JVMud so accounts can still connect while a new world is designed;
it is not part of the future setting.

## Preserved game systems

- account creation, authentication, and character persistence;
- Fighters, Rangers, Mages, and Clerics, including attributes and techniques;
- health, combat, recovery, experience, levels, training, and advancement;
- inventory, equipment, item blueprints, carrying capacity, and consumables;
- currency and economy functions;
- room speech, private tells, online presence, pronouns, and 100 social emotes;
- brief/full presentation and GMCP character and room support;
- reusable citizen, hostile, and quest-giver NPC templates; and
- an empty quest-catalog boundary ready for future story content.

The previous world-specific acceptance coverage was replaced with a focused
system-preservation suite against the login room.

## Native shape

- `jvmud/avelorn.config` declares the JVMud boundary and lifecycle vocabulary.
- `source/persona/` owns character policy and player-facing interaction.
- `source/place/login_room.c` is the temporary, non-world login Place.
- `source/system/` contains reusable mudlib services.
- `source/npc/` contains reusable NPC templates.
- `source/item/` contains the reusable item entity implementation.
- `accounts/` contains ignored runtime character snapshots.
- `docs/FOUNDATION.md` records the preserved technical contract.

## Development

Avelorn and JVMud are independent sibling projects. Clone JVMud beside this
repository, or set `JVMUD_ROOT` to its checkout. Install the current JVMud
snapshot before running Avelorn's acceptance suite:

```text
cd ../jvmud
mvn install
cd ../avelorn
mvn test
```

Run the game from the Avelorn repository root with:

```text
scripts/avelorn-start
```
