inherit "system/place";

mapping definition() {
  return ([
    "name": "Cinder Bank Reed Screen",
    "description":
        "A gap in the bracken exposes a low [door] in a side bank. You hear separate voices "
        + "from behind it and a single piece of iron laid on stone.",
    "details": ([
      "door":
          "The iron hinges face inside to avoid catching sunlight off the lake. A scuff on the "
          + "threshold shows the way someone last stepped out into the rain."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2292/y3897",
      "north": "place/cinder-bank/forge"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
