inherit "/item/item";

string query_blueprint() { return "weapon/steel-mace"; }

mapping item_profile() {
  return ([
    "name": "steel mace",
    "alias": "mace",
    "description": "A sturdy steel mace with a leather-wrapped grip.",
    "slot": "weapon",
    "weight": 6,
    "value": 150,
    "level": 1,
    "stat": "wisdom",
    "minimum": 11,
    "power": 3,
    "damage_type": "crush"
  ]);
}
