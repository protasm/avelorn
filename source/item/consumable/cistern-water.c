inherit "/item/item";

string query_blueprint() { return "consumable/cistern-water"; }

mapping item_profile() {
  return ([
    "name": "sealed bottle of cistern water",
    "alias": "water",
    "description": "A chip of red wax holds the reed stopper on a refillable clay bottle.",
    "slot": "none",
    "weight": 1,
    "value": 8,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 0,
    "travel_restore": 16
  ]);
}
