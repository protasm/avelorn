inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Nara Crook",
    "gender": "female",
    "role": "tidewall crew caller",
    "description": "Nara cannot call a new crew to the wall while the spillway still carries fresh tusk marks.",
    "aliases": ({ "nara", "caller" }),
    "quest": "hold-the-red-run"
  ]);
}
