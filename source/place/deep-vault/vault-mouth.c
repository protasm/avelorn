inherit "system/place";

mapping definition() {
  return ([
    "name": "Mouth of the Deep Vault",
    "description":
        "Two old haul roads converge. Their grades belong to the surface, but under the "
        + "[opening] the stone threads down in the same direction as the grain of a muscle.",
    "details": ([
      "opening":
          "The groove around the opening tightens midway down as if the rock grew around an "
          + "older slot. Fresh boot scuffs run against that grain."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2412/y3720",
      "down": "place/deep-vault/seam-guard"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
