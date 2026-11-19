inherit "/item/item";

string query_blueprint() { return "weapon/ironhold-crossbow"; }

mapping item_profile() {
  return ([
    "name": "Ironhold crossbow",
    "alias": "crossbow",
    "description": "Its plain, square nut is easy to replace with the tools of any bound company.",
    "slot": "weapon",
    "weight": 8,
    "value": 420,
    "level": 8,
    "stat": "dexterity",
    "minimum": 16,
    "power": 8,
    "damage_type": "pierce"
  ]);
}
