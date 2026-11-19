# Avelorn foundation contract

This document records Avelorn's runtime and persistence contracts. The current
setting and authoring rules are in the [lore](LORE.md) and [build plan](BUILD.md).

## Stable identifiers

Stable ids are lowercase slash-separated paths or lowercase hyphenated keys.
They are never display names.

- Launch classes are `fighter`, `ranger`, `mage`, and `cleric`.
- Launch race is `human`. Player genders are `male` and `female`.
- Places begin under `place/`.
- Item blueprint ids use lowercase category/name keys such as `weapon/arming-sword`.

Identifiers may gain aliases during migrations, but an identifier already
written to a character snapshot must not be silently reassigned.

## Character snapshot version 2

The save shape keeps account authentication and one character in the same LPC
object snapshot while preserving separate fields for their meanings. Durable
fields include account identity and password hash; character identity, gender,
and class; save format version; level and experience; six base attributes;
current and maximum class resources; currency; stable item blueprint IDs and
equipment assignments; a saved Place ID; quest progress; travel reserve; active
passage; and player preferences. Race and optional guild membership are saved
by stable ID. Existing saves without a race load with the founding default.

Only the authenticated owner can write a checkpoint. Two alternating generations
allow recovery after an interrupted write. On a legacy character's first
authenticated migration, the retired quest catalog is cleared once and valid
locations, equipment, and advancement are preserved. A withdrawn location falls
back to Westwatch. Reporting an assignment saves the reward and its claim together.

## Combat and advancement

Combatants publish level and health so players can evaluate risk. Level and
equipment recommendations remain soft gates that alter warnings and
effectiveness while leaving an attempt available. Present contributors can earn
victory credit through damage, healing, or protection. Defeated players recover
at a safe hearth and lose a modest portion of their money.

## Edition 1 wilderness movement

Wilderness Places use reusable LPC objects populated on demand from the
Edition 1 compressed JSON atlas. A logical Place path encodes one stable global
square coordinate. The room reports
its physical terrain classification, elevation, and exits. In verbose mode,
river descriptions distinguish headwaters, streams, rivers, and major rivers
from the frozen contributing area. One additional verbose sentence may call out
the most notable cardinal neighbor: water, a precipice, sharp relief, or a
terrain transition, in that order. Brief mode displays neither flavor sentence.

Walking considers only the four cardinal neighbors. An edge is passable when
the adjacent square exists, neither endpoint is open water, shallow lake margin,
river channel, cliff, or precipice, and the absolute elevation difference is no
more than 128 metres. The same rule applies in both directions. Runtime room and
region loading never instantiates the complete atlas at server startup.

Each class defines its own resource, starter weapon, attributes, techniques,
and styles. The advancement table defines XP thresholds and the cap. Items
define equipment slots and may implement their own LPC use behavior.

## Gender and language

Both player genders use the same character rules. Returning players with a
legacy gender value choose male or female after authentication. NPC templates
retain their pronoun keys. The shared pronoun service supplies grammatical
forms at presentation time.

## Native boundary

JVMud owns sessions, Players, Personas, presence, Places, Entities,
containment, movement, time, and persistence mechanics. Avelorn owns
authentication policy, character rules, combat, items, advancement, NPC
behavior, economy, and presentation.

Avelorn uses native lifecycle names such as `initialize`,
`offer_interactions`, `begin_session`, and `end_session`. New Java behavior is
appropriate only when it expresses a generally reusable JVMud capability.

## Filesystem-only persistence

Avelorn does not use a database. Accounts, characters, inventory blueprints,
equipment state, and future durable world checkpoints are stored beneath the
selected Avelorn mudlib root through JVMud's host-filesystem persistence
capability. The manifest must not request database access, and Avelorn source
must not call database engine functions.

Plaintext passwords must never reach durable fields. Runtime-only pending input
is cleared before every save.
