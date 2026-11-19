inherit "/item/item";

string query_blueprint() { return "weapon/concord-sword"; }

mapping item_profile() {
  return ([
    "name": "concord-edged sword",
    "alias": "concord-sword",
    "description": "A thin edge from the shared hearth runs along an otherwise ordinary steel blade.",
    "slot": "weapon",
    "weight": 6,
    "value": 420,
    "level": 8,
    "stat": "strength",
    "minimum": 15,
    "power": 7,
    "damage_type": "cut",
    "concord": 1
  ]);
}
