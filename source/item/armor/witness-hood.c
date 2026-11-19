inherit "/item/item";

string query_blueprint() { return "armor/witness-hood"; }

mapping item_profile() {
  return ([
    "name": "witness guard's riding hood",
    "alias": "hood",
    "description": "Small leather scales line the sides of this plain brown hood.",
    "slot": "head",
    "weight": 2,
    "value": 115,
    "level": 3,
    "stat": "dexterity",
    "minimum": 11,
    "protection": 2
  ]);
}
