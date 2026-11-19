inherit "/item/item";

string query_blueprint() { return "weapon/keeping-longbow"; }

mapping item_profile() {
  return ([
    "name": "keeping-house longbow",
    "alias": "longbow",
    "description": "The stave came from a mill grove under a hundred-year cutting agreement.",
    "slot": "weapon",
    "weight": 6,
    "value": 390,
    "level": 7,
    "stat": "dexterity",
    "minimum": 15,
    "power": 7,
    "damage_type": "pierce"
  ]);
}
