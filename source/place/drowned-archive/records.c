inherit "system/place";

mapping definition() {
  return ([
    "name": "First Recording Desk",
    "description":
        "A stone surface lies just high enough to write above the water. On the [edge], someone "
        + "cut a record of a flood too large for their witnesses to escape.",
    "details": ([
      "edge":
          "The archivist cut a handspan scale into the lip. A fresh wet line makes the current "
          + "level easy to read. Use `record waterline` during the weekly survey."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/drowned-archive/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-lowest-flood-mark-record",
        "verb": "record",
        "argument": "waterline",
        "text":
            "The water has advanced past the last dry mark but has not reached the writing "
            + "edge."
      ])
    })
  ]);
}
