inherit "system/place";

mapping definition() {
  return ([
    "name": "Tiled Resthouse",
    "description":
        "Two concentric rings of benches warm from a shared brick [chimney]. Older drain "
        + "workers play a counting game to test the judgment of whoever will clear the first "
        + "spring blockage.",
    "details": ([
      "chimney":
          "Its flue carries heat to both rings of benches. One cracked brick is set among sound "
          + "ones so trainees can find a fault without taking the chimney down."
    ]),
    "environment": "interior",
    "exits": ([
      "east": "place/keepingstead/square"
    ]),
    "services": ({ "rest" }),
    "npcs": ({ }),
    "safe": 1
  ]);
}
