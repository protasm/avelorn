inherit "/item/item";

string query_blueprint() { return "consumable/healing-draught"; }

mapping item_profile() {
  return ([
    "name": "minor healing draught",
    "alias": "draught",
    "description": "A stoppered clay vial of sharp-scented red cordial.",
    "slot": "none",
    "weight": 1,
    "value": 35,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 20,
    "travel_restore": 0
  ]);
}
