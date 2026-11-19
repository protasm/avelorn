inherit "system/place";

mapping definition() {
  return ([
    "name": "Old Signal Dome",
    "description":
        "The lamp's glass is crazed on the seaward side. A sheltered bench faces the little "
        + "harbor. An old [log] records calls sent across the water to Havenrode and storms "
        + "during which nobody answered.",
    "details": ([
      "log":
          "Several entries say simply no answer, then list the names of crews who went looking "
          + "anyway. The glass above the log still smells faintly of lamp oil."
    ]),
    "environment": "interior",
    "exits": ([
      "west": "place/headland-signal/lamp-walk"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
