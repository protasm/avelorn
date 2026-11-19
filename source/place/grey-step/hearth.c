inherit "system/place";

mapping definition() {
  return ([
    "name": "Oathless Kitchen",
    "description":
        "Beneath a smoke hole, a grill is slotted for as many pans as anyone wants to share. A "
        + "[painted stone] over the door records three refused levy shifts.",
    "details": ([
      "painted stone":
          "The stone would show four shifts, but the latest blank needs a witness before anyone "
          + "may mark it. A sliver of dry chalk is kept in the kitchen lintel."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/grey-step/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "fifth-crew-refusal" })
  ]);
}
