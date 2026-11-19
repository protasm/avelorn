inherit "system/place";

mapping definition() {
  return ([
    "name": "Seal Market",
    "description":
        "Frames of [wax] cool beside letter-knives and oak tally sticks. Apprentices buy their "
        + "first fair-weather cloaks from a stall kept by the public register binders.",
    "details": ([
      "wax":
          "The wax from the public register has a blue tint. A bare metal die lies beside it. "
          + "Use `inspect seal-die` to find what was stamped before any claim was made."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/witnessgate/square"
    ]),
    "services": ({ "market", "noticeboard" }),
    "npcs": ({ "callen-lint" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-uncut-seal-inspect",
        "verb": "inspect",
        "argument": "seal-die",
        "text":
            "An uncut die already bears the ministry's number. The maker had not put the "
            + "claimant's name on it."
      ])
    }),
    "quests": ({ "the-lowest-flood-mark" })
  ]);
}
