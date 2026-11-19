inherit "/item/item";

string query_blueprint() { return "armor/travel-cloak"; }

mapping item_profile() {
  return ([
    "name": "blue wool travel cloak",
    "alias": "cloak",
    "description": "A warm blue wool cloak with a plain brass clasp.",
    "slot": "body",
    "weight": 3,
    "value": 80,
    "level": 1,
    "stat": "constitution",
    "minimum": 10,
    "protection": 1
  ]);
}
