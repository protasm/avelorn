inherit "/item/item";

string query_blueprint() { return "consumable/boiler-tea"; }

mapping item_profile() {
  return ([
    "name": "flask of boiler tea",
    "alias": "tea",
    "description": "Lightly sweet tea in a leather-covered iron flask, brewed strong on the exchange boilers.",
    "slot": "none",
    "weight": 1,
    "value": 28,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 8,
    "travel_restore": 28
  ]);
}
