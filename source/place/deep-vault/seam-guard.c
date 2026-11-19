inherit "system/place";

mapping definition() {
  return ([
    "name": "Guard of the Seam",
    "description":
        "Winch blades lean away from a curved band of rock. A guard stands beside a coil of "
        + "black [linkage]. Farther in, someone tests a rope one tug at a time.",
    "details": ([
      "linkage":
          "Rounded lengths of black metal join without pins. When the guard shifts her weight, "
          + "one loop tightens before her foot reaches the ground."
    ]),
    "environment": "dungeon",
    "exits": ([
      "up": "place/deep-vault/vault-mouth",
      "east": "place/deep-vault/first-cut"
    ]),
    "services": ({ }),
    "npcs": ({ "vault-warden" }),
    "safe": 0
  ]);
}
