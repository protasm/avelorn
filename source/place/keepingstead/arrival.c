inherit "system/place";

mapping definition() {
  return ([
    "name": "Keepingstead Slip",
    "description":
        "A loading [sling] lifts sacks from flat-bottomed boats past a board warning against "
        + "mud on the paving. The square is north. The old Silt House is three squares west from "
        + "the coastal path out.",
    "details": ([
      "sling":
          "One loop is dyed blue to distinguish the backup sling from the one carrying the "
          + "day's reserve. Whichever takes the weight is written in the unloading ledger."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/keepingstead/square",
      "out": "place/atlas/x3033/y3856"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
