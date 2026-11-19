mapping definition() {
  return ([
    "name": "Record Vault",
    "nation": "ordel",
    "kind": "site",
    "anchor": ({ 2517, 4377 }),
    "entrance": "place/record-vault/approach",
    "control": "dominion",
    "institution": "An old witness archive was assigned to a garrison records office.",
    "economy": "Paper and witness seals carry the force of property.",
    "rooms": ([
      "place/record-vault/approach": ({ 30000039, "wilderness" }),
      "place/record-vault/works": ({ 30000040, "dungeon" }),
      "place/record-vault/records": ({ 30000041, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2517/y4377": ([
        "name": "Record Vault",
        "description":
            "Flood scratches at eye level show where the river once reached the archive. A "
            + "badly fitted door faces north, and wax has spilled over its sill."
      ])
    ]),
    "site": ([
      "area": "record-vault",
      "level": 4,
      "summary": "Reclaim the original witness roll from the keeper of the censor's seal."
    ])
  ]);
}
