mapping definition() {
  return ([
    "name": "Bond Yard",
    "nation": "arvenna",
    "kind": "site",
    "anchor": ({ 812, 3754 }),
    "entrance": "place/bond-yard/approach",
    "control": "dominion",
    "institution": "Sold-off shelter contracts supplied an occupied bonded barracks.",
    "economy": "Seized shelter debts, guard payroll, and records of lost guests.",
    "rooms": ([
      "place/bond-yard/approach": ({ 30000101, "wilderness" }),
      "place/bond-yard/works": ({ 30000102, "dungeon" }),
      "place/bond-yard/records": ({ 30000103, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x0812/y3754": ([
        "name": "Bond Yard",
        "description":
            "Iron spear rests stand outside the loan house. A shuttered window carries the "
            + "old symbol for an empty bed offered to a stranger."
      ])
    ]),
    "site": ([
      "area": "bond-yard",
      "level": 10,
      "summary": "Keep the retired contracts from returning to the debt market."
    ])
  ]);
}
