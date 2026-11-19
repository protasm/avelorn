inherit "system/place";

mapping definition() {
  return ([
    "name": "Lower Renewal Assembly",
    "description":
        "The floor bears four chalk lines for speakers, seconds, observers, and people who "
        + "decline to take a side. Water runs down the wall beside the [empty chair] of a speaker "
        + "seized in the last transit sweep.",
    "details": ([
      "empty chair":
          "A simple cut in the chair back marks the missing speaker's household. Use `read "
          + "minutes` to see what they said before the guard arrived."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/freeharbor/square"
    ]),
    "services": ({ }),
    "npcs": ({ "merev-call" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-chair-by-the-door-read",
        "verb": "read",
        "argument": "minutes",
        "text": "The minutes record that the speaker was present and expressly declined the pledge."
      ])
    })
  ]);
}
