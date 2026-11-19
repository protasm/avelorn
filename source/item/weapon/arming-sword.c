inherit "/item/item";

string query_blueprint() { return "weapon/arming-sword"; }

mapping item_profile() {
  return ([
    "name": "arming sword",
    "alias": "sword",
    "description": "A practical double-edged steel sword.",
    "slot": "weapon",
    "weight": 6,
    "value": 160,
    "level": 3,
    "stat": "strength",
    "minimum": 12,
    "power": 4,
    "damage_type": "cut"
  ]);
}
