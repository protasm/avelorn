mixed *areas() {
  return ({
    "westwatch", "headland-signal", "havenrode", "witnessgate", "keepingstead", "freeharbor",
    "ironhold", "salt-ledger", "record-vault", "silt-house", "tide-chapel", "empty-winch",
    "greenhame", "saltmere", "beacon-holt", "reedford", "whitequay", "orchard-cross", "eastweir",
    "saltbreak", "driftwood", "grey-step", "moorcross", "ropebridge", "lakeward", "coalstair",
    "cinder-bank", "deep-vault", "bond-yard", "drowned-archive", "red-run", "press-yard",
    "old-smelter"
  });
}

mapping regions() {
  return ([
    "arvenna": ([
      "name": "Arvenna",
      "historicalCode": 1,
      "people": "Arvenni",
      "institution": "Guest-house",
      "description":
          "An Arvenni roof shelters a guest and binds the host. Under occupation that promise "
          + "is a refuge, a debt, and sometimes an alibi."
    ]),
    "ordel": ([
      "name": "Ordel",
      "historicalCode": 2,
      "people": "Ordeli",
      "institution": "Witness court",
      "description":
          "In Ordel, a promise gains weight because someone stood and heard it. Two sets of "
          + "witnesses remember what the Dominion took."
    ]),
    "talvaren": ([
      "name": "Talvaren",
      "historicalCode": 3,
      "people": "Talvareni",
      "institution": "Keeping-house",
      "description":
          "Roads, drains, granaries, and the obligations that keep them working pass from one "
          + "season to the next. A broken sluice can starve a village as surely as a "
          + "requisition."
    ]),
    "caereth": ([
      "name": "Caereth",
      "historicalCode": 4,
      "people": "Caerethi",
      "institution": "Renewal assembly",
      "description":
          "On the south coast, a promise is worth giving because refusal is possible. The "
          + "Dominion can compel work more readily than it can secure an oath."
    ]),
    "durnath": ([
      "name": "Durnath",
      "historicalCode": 5,
      "people": "Durnathi",
      "institution": "Bound company",
      "description":
          "The old mining companies pledged to bring everyone on the rope home. Some carry "
          + "that promise in secret as the Dominion demands obedience in its name."
    ])
  ]);
}

string starting_place() {
  return "place/westwatch/guest-house";
}
