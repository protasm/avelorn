inherit "system/place";

mapping definition() {
  return ([
    "name": "Moor Harness Stall",
    "description":
        "The [felt girths] hang back to back to dry. A blacksmith uses rejected pins from the "
        + "wagon road to repair a drover's lantern.",
    "details": ([
      "felt girths":
          "A bill for two animals is pinned behind the drying girths, though the traces show "
          + "different brands. Use `split bill` after checking who lent each animal."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/moorcross/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "pay-the-drover-split",
        "verb": "split",
        "argument": "bill",
        "text":
            "A separate receipt credits both lenders and keeps the extra haul from becoming a "
            + "new obligation."
      ])
    })
  ]);
}
