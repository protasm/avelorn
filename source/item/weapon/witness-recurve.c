inherit "/item/item";

string query_blueprint() { return "weapon/witness-recurve"; }

mapping item_profile() {
  return ([
    "name": "witness court recurve",
    "alias": "recurve",
    "description": "A witness has signed the pale horn belly rather than marking the string's owner.",
    "slot": "weapon",
    "weight": 4,
    "value": 225,
    "level": 3,
    "stat": "dexterity",
    "minimum": 13,
    "power": 5,
    "damage_type": "pierce"
  ]);
}
