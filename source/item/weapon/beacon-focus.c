inherit "/item/item";

string query_blueprint() { return "weapon/beacon-focus"; }

mapping item_profile() {
  return ([
    "name": "beacon glass wand",
    "alias": "wand",
    "description": "An old signal lens sits in a resin-dark handle.",
    "slot": "weapon",
    "weight": 2,
    "value": 210,
    "level": 3,
    "stat": "intelligence",
    "minimum": 13,
    "power": 5,
    "damage_type": "arcane"
  ]);
}
