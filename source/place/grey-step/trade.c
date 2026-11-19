inherit "system/place";

mapping definition() {
  return ([
    "name": "Grey Step Workshop",
    "description":
        "A boat stands bow-up on slats. Beside its [keel], a young rigger offers messenger "
        + "packs threaded with news of the latest refusal.",
    "details": ([
      "keel":
          "A nick in the keel holds the crew's own chalk out of sight of the courier. Use "
          + "`recover chalk` to bring it to the refusal record."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/grey-step/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "fifth-crew-refusal-recover",
        "verb": "recover",
        "argument": "chalk",
        "text": "You take a nub of the crew's own marking chalk from under the waiting boat."
      ])
    })
  ]);
}
