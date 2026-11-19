inherit "system/place";

mapping definition() {
  return ([
    "name": "Court of the Third Ear",
    "description":
        "Three plain [stools] wait beneath a long beam. Two are for the parties and one for "
        + "whoever heard them. An iron box bears an inventory of records sent elsewhere and never "
        + "returned.",
    "details": ([
      "stools":
          "The witness's stool is the only one built without arms. Use `hear cutter` while it "
          + "is occupied or `submit witness-note` here with the flood reading."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/witnessgate/square"
    ]),
    "services": ({ }),
    "npcs": ({ "senna-blade" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-uncut-seal-hear",
        "verb": "hear",
        "argument": "cutter",
        "text":
            "The cutter confirms in front of the third stool that the ministry supplied the "
            + "die."
      ]),
      ([
        "tag": "the-lowest-flood-mark-submit",
        "verb": "submit",
        "argument": "witness-note",
        "text": "The witness writes your measurement beneath the occupied court's blank column."
      ])
    })
  ]);
}
