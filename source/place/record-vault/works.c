inherit "system/place";

mapping definition() {
  return ([
    "name": "Seal Room",
    "description":
        "An [iron box] holds original seals. Split shelves reveal that some inscriptions were "
        + "carried sideways through the archive to preserve the older order of events.",
    "details": ([
      "iron box":
          "Provision was made for a lock, but the slots have been filed smooth from within. "
          + "Before the garrison, a witness could open it while another watched."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/record-vault/approach",
      "down": "place/record-vault/records"
    ]),
    "services": ({ }),
    "npcs": ({ "seal-thief" }),
    "safe": 0
  ]);
}
