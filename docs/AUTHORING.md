# Authoring Avelorn

Author a locality in LPC. The frozen Edition 1 atlas supplies terrain, walking edges, and coordinates. The historical atlas assigns each square to one of the five pre-Conquest nations. Choose a physically walkable anchor, then use the area's `control`, livelihood, and institutions to show what is happening there now. [North Island lore](LORE.md) · [Historical borders](../atlas/political/README.md)

## Register a locality

Add its stable ID to `areas()` in [the area registry](../source/area/registry.c). Give it a `definition()` in `source/area/<id>/area.c`. Index every local room by its full Place path, explicit GMCP number, and environment. Write each room's description and exits in a corresponding `source/place/<id>/<room>.c` that inherits `system/place`. Room files are loaded when needed, and their ancestors supply movement, commands, scenery examination, and services.

Choose lowercase hyphenated locality, Place, route, quest, and resident IDs. Object references omit the `.c` suffix. Keep existing IDs and GMCP numbers stable across prose and implementation changes. Saved locations and sanctuaries use Place paths, quest journals use quest IDs, and pending voyages use route IDs and destination paths. Assign new authored GMCP numbers at or above `30000000` and check uniqueness across the complete registry. Atlas squares already have numbers derived from their coordinates.

The example below adds a small Arvenni site called Rescue Rail one square north of Westwatch. Use the values as an example and verify the anchor and GMCP number are available in your checkout.

```c
// source/area/rescue-rail/area.c
mapping definition() {
  return ([
    "name": "Rescue Rail",
    "nation": "arvenna",
    "kind": "site",
    "anchor": ({ 113, 3337 }),
    "entrance": "place/rescue-rail/watch",
    "control": "contested",
    "institution": "Ferry households take turns watching for capsized boats.",
    "economy": "Crew contributions maintain the rescue gear.",
    "rooms": ([
      "place/rescue-rail/watch": ({ 30009001, "wilderness" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x0113/y3337": ([
        "name": "Rescue Rail Path",
        "description": "A short path leads to a rail above the surf."
      ])
    ]),
    "site": ([
      "area": "rescue-rail",
      "level": 1,
      "summary": "Check the shore crew's rescue line before the next crossing."
    ])
  ]);
}
```

## Write the room and its scenery

Give the room one or two short paragraphs. Weave an inspectable label such as `[cleat]` into the sentence, and supply a corresponding key in `details`. Use concise lowercase labels, one occurrence per label, and match every marker with exactly one examination response. Give readers a fresh observation or a clue to a local action. Players see plain prose by default. They can type `details`, `look cleat`, or `exa cleat` in either hint mode. `hints on` shows brackets and `hints off` hides them. The setting persists, and brief mode keeps a compact list of details.

Use the same `environment` as the area index. `wilderness` applies the travel governor and reserve cost where applicable. Use the room's `services` list for existing features such as `rest`, `market`, `training`, `noticeboard`, and `transit`. Set `safe` to `1` for a recovery room with `rest`. For a locality's entrance, use an `out` exit to its anchor. The atlas square automatically offers `enter`.

```c
// source/place/rescue-rail/watch.c
inherit "system/place";

mapping definition() {
  return ([
    "name": "Rescue Rail Watch",
    "description": "A rope runs from a bronze [cleat] through the seaward rail.",
    "details": ([
      "cleat": "Fresh salt hides a loose turn. Use check rope to test the line."
    ]),
    "environment": "wilderness",
    "exits": ([ "out": "place/atlas/x0113/y3337" ]),
    "services": ({ }),
    "npcs": ({ "rescue-keeper" }),
    "safe": 0,
    "actions": ({
      ([
        "verb": "check",
        "argument": "rope",
        "tag": "rescue-rail-check",
        "text": "You pull the loose turn snug and test the line under load."
      ])
    })
  ]);
}
```

## Connect the travel graph

Add a `westwatch-rescue-rail` entry inside the new area's `trails` mapping. The `from` field can refer to an existing area. In this case the origin and destination anchors are adjacent. For longer trails, verify every cardinal edge using the actual atlas and mark intermediate sections with places, landmarks, or decisions the player can recognize. A useful description names a direction or visible waypoint.

```c
"westwatch-rescue-rail": ([
  "from": "westwatch",
  "to": "rescue-rail",
  "steps": ({ "north" }),
  "description": "Follow the dunes one square north of Westwatch Landing."
])
```

Use reciprocal exits for paths inside a locality. For a staffed connection between settlements, place an outgoing route in each departure locality and give each direction a unique ID. Set `from` and `to` to the terminals' Place paths, list `transit` at those entrances, and supply `name`, `mode`, `duration` in seconds, `fare` in copper, and a `description` for boarding. Keep legs at two minutes or less and preserve free connections within three legs between Westwatch and the main hubs. Confirm each settlement has a free way in and back out. [Blue Lamp Ferry definition](../source/area/westwatch/area.c)

## Add a resident and an assignment

Put `rescue_keeper.c` under `source/npc/rescue-rail/` and register `"rescue-keeper": "npc/rescue-rail/rescue_keeper"` in [the creature registry](../source/system/creatures.c). The room's `npcs` list refers to the registry ID. For a board-issued assignment, give the room `noticeboard` service and put the ID in its `quests` list. Use one offering location per assignment.

```c
inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Nella Reed",
    "gender": "female",
    "role": "keeper of the rescue rail",
    "description": "Nella tracks when the shore crews last checked the line.",
    "aliases": ({ "nella", "keeper" }),
    "quest": "check-the-rescue-line"
  ]);
}
```

Add the following quest in `source/quest/rescue-rail/check_the_rescue_line.c`. Register `"check-the-rescue-line": "quest/rescue-rail/check_the_rescue_line"` in [the quest catalog](../source/system/quests.c), then set the resident's `quest` to `check-the-rescue-line`.

```c
mapping definition() {
  return ([
    "title": "Check the Rescue Line",
    "description": "At the rail, check rope and report to the keeper.",
    "level": 1,
    "count": 1,
    "xp": 25,
    "copper": 5,
    "action_tag": "rescue-rail-check",
    "repeatable": 0
  ]);
}
```

Match a room action's tag to the quest's `action_tag`. Match a hostile's `defeatTag` to a quest's `defeat_tag`. Use globally unique objective tags and the matching tag arrays for distinct objectives in a longer assignment. Keep `count` consistent with those objectives and award gear by stable blueprint ID. `level` is a recommendation. A `prerequisite` requires the player to have reported the preceding quest.

## Extend characters and gear

Add a class file under `source/class/` and list its stable ID in [the class registry](../source/class/registry.c). Its profile defines a primary stat, starting attribute overrides, a starter weapon ID, and the class resource. It also lists styles and techniques in their display order. A technique can reuse an existing effect or supply a `handler` path to an LPC `perform(object actor, string target, int power)` method. Return success after applying the effect. The Persona owns the level check, resource cost, and action limit.

Register a race in [the race registry](../source/race/registry.c) and give it an LPC definition with a name and description. Keep its stable ID distinct from nationality and affiliation. The creation flow offers a choice when more than one race is registered. Legacy saves keep the founding default.

For a new item type, add an LPC leaf under the relevant item category and register its blueprint ID. A new equipment slot can be named in the item profile. A leaf can override `int use_by(object actor)` to add its own action. Return 0 for an unsupported action, 1 for success with the item retained, or 2 for success with the item consumed. Keep the established blueprint IDs for existing saves.

## Extend levels or guilds

The [progression table](../source/system/progression.c) gives each level a row of `({ experience to reach this level, training points awarded })`. Level 1 is the starting row. Append higher levels in ascending order with a positive experience requirement, and choose their training awards explicitly. The number of rows determines `cap()`. Combat and content use `experience_to_advance(current_level)` and `training_points_for(attained_level)` for the two values. Preserve published levels and their meaning for saved characters. When changing the cap, review item and encounter ranges, endgame entry conditions, and acceptance tests for both existing and extended characters.

The launch [guild registry](../source/guild/registry.c) is empty. To define a guild, add a stable ID and its LPC provider path to `registrations()`. For example, `"bridge-company": "guild/bridge-company/charter"` refers to `source/guild/bridge-company/charter.c`. Return a `mapping definition()` with a nonempty `name` and `description`. An optional `int eligible(object persona)` can consult the character's level or other Persona APIs and return whether the character may join. Providers without that hook accept any supplied Persona. Guild content can call `join_guild(id)` or `leave_guild()` on the player after a local interaction. The Persona applies eligibility and saves the resulting membership by ID.

## Verify and iterate

Run these commands from the Avelorn checkout with the pinned JVMud build installed. `scripts/avelorn-install-jvmud` sets up that build from the sibling JVMud checkout when needed.

```sh
mvn -B -Dtest=AvelornContentTest,AvelornExpansionTest,AvelornCampaignTest test
mvn -B test
```

Native content tests compile room definitions and check atlas anchors and paths. The expansion and campaign cases use isolated account stores. To play the example, restart your development worker, then enter from Westwatch with `walk south out north enter`. Run `work check-the-rescue-line`, `details`, `look cleat`, `check rope`, and `report check-the-rescue-line`. Return with `walk out south enter north`. Recheck with hints on, and relog with a saved character to verify continuity.

Restart the worker after editing registries, room definitions, or resident definitions. The worker builds indexes at startup and initialized Places hold their configured room data. Use a disposable local account for manual tests and preserve release saves across restarts. [Build and release gates](BUILD.md)
