inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Warden Senk",
    "gender": "female",
    "role": "enemy",
    "description":
        "Senk's black armor anticipates the weight of her blade. Beneath each shoulder plate, " +
        "a second line of darker metal runs into the stone.",
    "aliases": ({ "senk", "warden" }),
    "level": 10,
    "health": 160,
    "minimumDamage": 6,
    "maximumDamage": 10,
    "experience": 290,
    "copper": 65,
    "defeatTag": "vault-warden",
    "bonded": 1
  ]);
}
