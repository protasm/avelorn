inherit "system/place";

mapping definition() {
  return ([
    "name": "Foster Hall",
    "description":
        "A row of low [pegs] bears child-sized coats. The cook slices a loaf according to the "
        + "same roster that assigns ditch work, placing new names between familiar ones.",
    "details": ([
      "pegs":
          "Bright dents in the plaster mark where unoccupied pegs were pulled out. Use `audit "
          + "bunks` to put the spare places back on the roster."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/greenhame/arrival",
      "down": "place/greenhame/list-room"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "sella-fane" }),
    "safe": 1,
    "quests": ({ "an-empty-bunk" }),
    "actions": ({
      ([
        "tag": "an-empty-bunk-audit",
        "verb": "audit",
        "argument": "bunks",
        "text":
            "The stray nails show three empty pegs. You enter three spaces in the current "
            + "roster."
      ])
    })
  ]);
}
