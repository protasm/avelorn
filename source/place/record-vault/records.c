inherit "system/place";

mapping definition() {
  return ([
    "name": "The Second Register",
    "description":
        "Ordeli witnesses wrote down two [records] of the same seizure. In one, no household "
        + "ever occupied the land. In the other, a dozen people named their children before "
        + "losing it.",
    "details": ([
      "records":
          "The handwriting changes halfway down the older copy. Use `compare records` to note "
          + "exactly where the two witnesses' dates differ."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/record-vault/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "two-points-of-ink-compare",
        "verb": "compare",
        "argument": "records",
        "text":
            "Twelve identical lists diverge on one date. You keep both dates and mark the "
            + "handwriting."
      ])
    })
  ]);
}
