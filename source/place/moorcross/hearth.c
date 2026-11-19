inherit "system/place";

mapping definition() {
  return ([
    "name": "Lambing Shed Hall",
    "description":
        "A rough [shelf] holds the old shareholder names and a new list of workers who never "
        + "owned stock. Someone has taken care to write both lists in equally large letters.",
    "details": ([
      "shelf":
          "The two lists rest side by side under a shared piece of glass. Foremen who want "
          + "extra labor have to ask for one of the names on the second list."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/moorcross/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "pay-the-drover" })
  ]);
}
