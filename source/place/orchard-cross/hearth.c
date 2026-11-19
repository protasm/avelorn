inherit "system/place";

mapping definition() {
  return ([
    "name": "Graft Hall",
    "description":
        "Colored straw knots mark rootstock supplied by each village. A [public shelf] carries "
        + "instructions for replacing the trees killed by drainage from the newest road.",
    "details": ([
      "public shelf":
          "An old deed is tucked under the repair instructions, where a displaced worker's "
          + "claim was still recorded. Use `review deed` to see who inherits the graft."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/orchard-cross/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "lena-sot" }),
    "safe": 1,
    "quests": ({ "giving-a-share-back" }),
    "actions": ({
      ([
        "tag": "giving-a-share-back-review",
        "verb": "review",
        "argument": "deed",
        "text":
            "The original deed names a successor in the displaced household rather than its "
            + "absent owner."
      ])
    })
  ]);
}
