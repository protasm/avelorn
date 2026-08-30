# Avelorn foundation contract

This document records the mechanics retained across the world and story reset.
It deliberately establishes no setting, geography, factions, campaign, named
characters, or quests.

## Stable identifiers

Stable ids are lowercase slash-separated paths or lowercase hyphenated keys.
They are never display names.

- Classes: `fighter`, `ranger`, `mage`, `cleric`.
- Genders: `male`, `female`, `non-binary`.
- Places begin under `place/`.
- Item blueprint ids use lowercase category/name keys such as `weapon/arming-sword`.

Identifiers may gain aliases during migrations, but an identifier already
written to a character snapshot must not be silently reassigned.

## Character snapshot version 1

The save shape keeps account authentication and one character in the same LPC
object snapshot while preserving separate fields for their meanings. Durable
fields include account identity and password hash; character identity, gender,
and class; save format version; level and experience; six base attributes;
current and maximum class resources; currency; stable item blueprint ids and
equipment assignments; and a saved Place id.

The previous quest state is discarded when an existing account next logs in.
All other character state remains intact. During the reset, all characters enter
the temporary `place/login_room` because former saved Place ids no longer
exist.

## Combat and advancement

Combatants publish level and health so players can evaluate risk. Level and
equipment recommendations remain soft gates that alter warnings and
effectiveness without prohibiting an attempt. Every contributor still present
when an opponent falls receives victory credit. Defeat is recoverable: the
character returns to the login room, resources recover, and the existing
small coin loss remains in effect.

Each class retains its resource-powered technique. Experience, levels,
attribute improvement, starter equipment, inventory, equipment, consumables,
currency, and training remain available.

## Gender and language

Gender never changes attributes, classes, equipment, or progression. Personas
and NPC templates use one of the three stable gender keys. The shared pronoun
service supplies grammatical forms at presentation time. Stored prose must not
bake in a character's pronouns.

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
