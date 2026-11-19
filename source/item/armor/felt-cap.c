inherit "/item/item";

string query_blueprint() { return "armor/felt-cap"; }

mapping item_profile() {
  return ([
    "name": "quilted felt cap",
    "alias": "cap",
    "description": "Grey, close-stitched felt takes the edge off a glancing blow.",
    "slot": "head",
    "weight": 1,
    "value": 50,
    "level": 1,
    "stat": "constitution",
    "minimum": 10,
    "protection": 1
  ]);
}
