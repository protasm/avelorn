inherit "/item/item";

string query_blueprint() { return "armor/keeping-brigandine"; }

mapping item_profile() {
  return ([
    "name": "keeping-house brigandine",
    "alias": "brigandine",
    "description": "The oiled lining covers plates cut from a disused granary door.",
    "slot": "body",
    "weight": 9,
    "value": 310,
    "level": 6,
    "stat": "constitution",
    "minimum": 13,
    "protection": 5
  ]);
}
