inherit "/item/item";

string query_blueprint() { return "armor/tallow-wraps"; }

mapping item_profile() {
  return ([
    "name": "tallow-cured work wraps",
    "alias": "wraps",
    "description": "Thick hide wraps the wrists without obstructing the grip.",
    "slot": "hands",
    "weight": 1,
    "value": 70,
    "level": 1,
    "stat": "constitution",
    "minimum": 10,
    "protection": 1
  ]);
}
