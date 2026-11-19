inherit "system/place";

mapping definition() {
  return ([
    "name": "Low Reserve Stores",
    "description":
        "Raised shelves hold empty [crates] promised to flood victims. Grain under cloth is "
        + "labeled for three villages that have already sent what they could spare.",
    "details": ([
      "crates":
          "The crate labels give dry sacks precedence over wet ones in the next allotment. Use "
          + "`tally sacks` here before marking the household map."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/silt-house/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "dry-sacks-tally",
        "verb": "tally",
        "argument": "sacks",
        "text": "You assign one chalk number to every dry sack and leave the wet ones uncounted."
      ])
    })
  ]);
}
