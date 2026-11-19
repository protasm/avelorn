inherit "/item/item";

string query_blueprint() { return "weapon/ashwood-shortbow"; }

mapping item_profile() {
  return ([
    "name": "ashwood shortbow",
    "alias": "bow",
    "description": "A compact bow made from layered ash and horn.",
    "slot": "weapon",
    "weight": 4,
    "value": 145,
    "level": 1,
    "stat": "dexterity",
    "minimum": 11,
    "power": 3,
    "damage_type": "pierce"
  ]);
}
