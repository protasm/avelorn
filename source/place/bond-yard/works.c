inherit "system/place";

mapping definition() {
  return ([
    "name": "Guard Tenement",
    "description":
        "The [cupboard] holds pile after pile of canceled guest debts. A black-armed guard "
        + "travels with anyone who comes to inspect them.",
    "details": ([
      "cupboard":
          "One bundle of canceled debts bears a new transfer stamp. The guard has placed it "
          + "within reach of the door, ready for another buyer."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/bond-yard/approach",
      "down": "place/bond-yard/records"
    ]),
    "services": ({ }),
    "npcs": ({ "ledger-enforcer" }),
    "safe": 0
  ]);
}
