inherit "/item/item";

string query_blueprint() { return "consumable/salt-fish"; }

mapping item_profile() {
  return ([
    "name": "packet of dry salt fish",
    "alias": "fish",
    "description": "Salt and paper keep these thin, oily pieces edible through a long day's travel.",
    "slot": "none",
    "weight": 1,
    "value": 22,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 6,
    "travel_restore": 24
  ]);
}
