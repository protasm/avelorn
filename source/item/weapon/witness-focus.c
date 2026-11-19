inherit "/item/item";

string query_blueprint() { return "weapon/witness-focus"; }

mapping item_profile() {
  return ([
    "name": "witness-cut focus",
    "alias": "focus",
    "description": "The clear stone has four polished faces so the bearer can read the grain from each side.",
    "slot": "weapon",
    "weight": 3,
    "value": 320,
    "level": 5,
    "stat": "intelligence",
    "minimum": 14,
    "power": 6,
    "damage_type": "arcane"
  ]);
}
