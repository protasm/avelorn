inherit "system/place";

mapping definition() {
  return ([
    "name": "Company Return Shelf",
    "description":
        "Behind a shelf of broken catches, the company's unlicensed [rescue line] runs out "
        + "through a narrow crack. Several hands on that line pull hard enough to keep it taut.",
    "details": ([
      "rescue line":
          "Tugs come in groups of three, then pause for a reply. The rope has been spliced in "
          + "several hands, with ordinary fibers from different companies."
    ]),
    "environment": "interior",
    "exits": ([
      "west": "place/deep-vault/first-cut"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
