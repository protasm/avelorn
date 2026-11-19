# Avelorn design documentation

This folder is the project home for recorded design decisions, worldbuilding,
and technical foundations. Keep decisions here so future work can continue from
the accepted setting rather than reconstructing it from conversation history.

## Current references

- [Area authoring guide](AUTHORING.md) walks through adding an LPC locality, its travel links, and a quest.
- [Avelorn 1.0 build plan](BUILD.md) records the playable scope, LPC authoring rules, and release gates.
- [North Island lore](LORE.md): the five named nations (Arvenna, Ordel, Talvaren,
  Caereth, and Durnath), their cultural foundations, the Durnathi Dominion,
  the Conquest, the corrupting resource, and the emerging resistance. Records
  approved working decisions alongside explicitly provisional or deferred details.
- [Technical foundation](FOUNDATION.md): the preserved technical contract and
  foundation of the game.

## Related design assets

- [Working pre-Conquest borders](../atlas/political/pre-conquest/working-borders.json):
  the authoritative pointer to the adopted territorial dataset.
- [Political atlas documentation](../atlas/political/README.md): ownership data,
  coordinate conventions, and versioning.
- [Tiled atlas](../atlas/tiled/README.md) describes the map view and the frozen physical terrain.

These datasets and images remain in their existing project locations; this
index links to them rather than creating competing copies.

## Recording future decisions

- Update the relevant document when a decision is accepted, noting the revision
  date and removing or qualifying statements it supersedes.
- Distinguish accepted working decisions from proposals, examples, open
  questions, and deliberately deferred work.
- Add a focused document and link it here when a subject becomes substantial
  enough to need its own reference.
- Keep narrative decisions separate from implementation status. Recording lore
  does not mean it is implemented in live gameplay.
- Preserve adopted atlas versions; geographic changes follow the political
  descriptor's revision policy rather than silently rewriting old snapshots.
