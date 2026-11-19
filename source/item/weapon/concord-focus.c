inherit "/item/item";

string query_blueprint() { return "weapon/concord-focus"; }

mapping item_profile() {
  return ([
    "name": "concord ring focus",
    "alias": "concord-focus",
    "description": "A small concord ring holds the glass without enclosing it, leaving a gap that answers to intention.",
    "slot": "weapon",
    "weight": 4,
    "value": 420,
    "level": 8,
    "stat": "intelligence",
    "minimum": 15,
    "power": 7,
    "damage_type": "arcane",
    "concord": 1
  ]);
}
