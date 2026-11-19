inherit "system/place";

mapping definition() {
  return ([
    "name": "Reriggers' Row",
    "description":
        "Bent rivets and spliced spars bear the names of three previous owners. One crew laughs "
        + "because a salvage judge just pronounced its hardest-working [pulley] beyond repair.",
    "details": ([
      "pulley":
          "Old nail holes in the pulley tag match a strip of the charter. Use `tally salvage` "
          + "to check ownership or `mend charter` to line up the surviving strips."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/driftwood/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "action": ([
      "tag": "tally-salvage-marks",
      "verb": "tally",
      "argument": "salvage",
      "text":
          "You tally the old mast marks against the crews' ledger. The disputed pile was "
          + "brought up by the fisher crews."
    ]),
    "actions": ({
      ([
        "tag": "a-charter-in-scraps-mend",
        "verb": "mend",
        "argument": "charter",
        "text": "The matching nail holes join strips that had been treated as separate contracts."
      ])
    })
  ]);
}
