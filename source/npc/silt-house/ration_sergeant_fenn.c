inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Ration Sergeant Fenn",
    "gender": "male",
    "role": "enemy",
    "description":
        "Fenn has locked the wardens' tools in the gate cage. A band of black metal slides " +
        "across his bracer when he moves.",
    "aliases": ({ "fenn", "sergeant" }),
    "level": 5,
    "health": 65,
    "minimumDamage": 4,
    "maximumDamage": 7,
    "experience": 120,
    "copper": 29,
    "defeatTag": "ration-sergeant"
  ]);
}
