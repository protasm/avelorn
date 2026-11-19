inherit "system/place";

mapping definition() {
  return ([
    "name": "The Spare Stoop",
    "description":
        "This narrow hostel leaves a [lamp] burning above the street so a witness can reach "
        + "home after a late hearing. Tea is always warm and nobody takes a name at the door.",
    "details": ([
      "lamp":
          "A stamp hangs inside the lantern bracket, free for a second witness to use. While "
          + "preparing the archived testimony, use `seal transcript` at the hostel."
    ]),
    "environment": "interior",
    "exits": ([
      "east": "place/witnessgate/square"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "two-points-of-ink" }),
    "actions": ({
      ([
        "tag": "two-points-of-ink-seal",
        "verb": "seal",
        "argument": "transcript",
        "text": "Two witnesses seal the transcript under separate impressions."
      ])
    })
  ]);
}
