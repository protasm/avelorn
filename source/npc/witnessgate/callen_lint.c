inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Callen Lint",
    "gender": "male",
    "role": "witness timber restorer",
    "description": "Callen repairs the records that survive wet ink, woodworm, deliberate censure, and the tide.",
    "aliases": ({ "callen", "restorer" }),
    "quest": "dry-the-record"
  ]);
}
