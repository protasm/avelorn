inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Aften Vale",
    "gender": "male",
    "role": "reserve keeper",
    "description":
        "Aften must restore a pressure gate without flooding the low granary or giving the " +
        "garrison a reason to seize the works.",
    "aliases": ({ "aften", "keeper" }),
    "quest": "a-storehouse-in-trust"
  ]);
}
