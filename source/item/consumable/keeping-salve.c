inherit "/item/item";

string query_blueprint() { return "consumable/keeping-salve"; }

mapping item_profile() {
  return ([
    "name": "vial of keeping-house salve",
    "alias": "salve",
    "description": "An open palm stamped in green wax identifies the work of the keeping-house healers.",
    "slot": "none",
    "weight": 1,
    "value": 60,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 42,
    "travel_restore": 0
  ]);
}
