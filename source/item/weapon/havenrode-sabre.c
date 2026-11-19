inherit "/item/item";

string query_blueprint() { return "weapon/havenrode-sabre"; }

mapping item_profile() {
  return ([
    "name": "Havenrode sabre",
    "alias": "sabre",
    "description": "The swept blade and open guard are favored by watchkeepers on the wharf.",
    "slot": "weapon",
    "weight": 5,
    "value": 220,
    "level": 3,
    "stat": "strength",
    "minimum": 13,
    "power": 5,
    "damage_type": "cut"
  ]);
}
