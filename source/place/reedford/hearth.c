inherit "system/place";

mapping definition() {
  return ([
    "name": "Matters Table",
    "description":
        "A low ceiling keeps the lamp heat down near the benches. Anyone may place an [eel "
        + "bone] on the table to demand a witness before the next wetland crossing.",
    "details": ([
      "eel bone":
          "A worn groove tells where a petitioner should set the bone. Witnesses stand on the "
          + "side nearest the door so the requester can leave first."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/reedford/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "when-the-causeway-fell" })
  ]);
}
