inherit "system/place";

mapping definition() {
  return ([
    "name": "Salt Ledger Counting-House",
    "description":
        "The [registers] lie open on brass racks. The pages distinguish the salt due on a crate "
        + "from the charge due on a runaway apprentice.",
    "details": ([
      "registers":
          "A column for missing guests has the same heading as the column for damaged cargo. "
          + "The newest charges are entered in the same black ink."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/salt-ledger/approach",
      "down": "place/salt-ledger/records"
    ]),
    "services": ({ }),
    "npcs": ({ "bond-broker" }),
    "safe": 0
  ]);
}
