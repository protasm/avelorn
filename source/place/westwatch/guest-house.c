inherit "system/place";

mapping definition() {
  return ([
    "name": "Westwatch Guest-House",
    "description":
        "Someone has set a bowl and a clean spoon beside the hearth. Households send blankets "
        + "here when the sailing is bad. Iona keeps a blue [lamp] in the window so travelers can "
        + "find the door.\n\nThe landing is south, the cooper's yard is east, and an old signal "
        + "book sits on the [noticeboard].",
    "details": ([
      "lamp":
          "Two panes of blue glass still carry a fisher's scratched initials. A boat coming "
          + "around the point sees this light before it sees a roof.",
      "noticeboard":
          "The signal book and the shelter register hang from the same peg. For Iona's guest "
          + "tally, use `sign register`."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/westwatch/landing",
      "east": "place/westwatch/coopers-yard"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "iona-venn" }),
    "safe": 1,
    "quests": ({ "three-shared-bowls" }),
    "actions": ({
      ([
        "tag": "three-shared-bowls-sign",
        "verb": "sign",
        "argument": "register",
        "text": "You sign beside the households that put bowls out for strangers."
      ])
    })
  ]);
}
