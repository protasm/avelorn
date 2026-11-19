inherit "/item/item";

string query_blueprint() { return "consumable/orchard-cordial"; }

mapping item_profile() {
  return ([
    "name": "orchard cordial",
    "alias": "cordial",
    "description": "Crushed red orchard fruit sweetens the bitter contents of this medicine bottle.",
    "slot": "none",
    "weight": 1,
    "value": 45,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 28,
    "travel_restore": 0
  ]);
}
