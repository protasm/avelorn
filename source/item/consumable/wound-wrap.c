inherit "/item/item";

string query_blueprint() { return "consumable/wound-wrap"; }

mapping item_profile() {
  return ([
    "name": "clean wound wrap",
    "alias": "bandage",
    "description": "A strip of boiled linen with a narrow clasp for one-handed binding.",
    "slot": "none",
    "weight": 1,
    "value": 20,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 12,
    "travel_restore": 0
  ]);
}
