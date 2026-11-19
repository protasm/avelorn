inherit "/item/item";

string query_blueprint() { return "weapon/freeharbor-spear"; }

mapping item_profile() {
  return ([
    "name": "Freeharbor boarding spear",
    "alias": "spear",
    "description": "A leaf-shaped point grips the red cord wound around the narrow shaft.",
    "slot": "weapon",
    "weight": 5,
    "value": 310,
    "level": 5,
    "stat": "strength",
    "minimum": 14,
    "power": 6,
    "damage_type": "pierce"
  ]);
}
