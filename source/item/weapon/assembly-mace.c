inherit "/item/item";

string query_blueprint() { return "weapon/assembly-mace"; }

mapping item_profile() {
  return ([
    "name": "assembly mace",
    "alias": "assembly-mace",
    "description": "The head can be unscrewed and presented at a renewal as a token of service.",
    "slot": "weapon",
    "weight": 5,
    "value": 325,
    "level": 6,
    "stat": "wisdom",
    "minimum": 14,
    "power": 6,
    "damage_type": "crush"
  ]);
}
