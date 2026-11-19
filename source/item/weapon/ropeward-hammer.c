inherit "/item/item";

string query_blueprint() { return "weapon/ropeward-hammer"; }

mapping item_profile() {
  return ([
    "name": "ropeward war hammer",
    "alias": "war-hammer",
    "description": "The rope binding recalls workers who did not come back from the lower galleries.",
    "slot": "weapon",
    "weight": 8,
    "value": 420,
    "level": 8,
    "stat": "wisdom",
    "minimum": 16,
    "power": 8,
    "damage_type": "crush"
  ]);
}
