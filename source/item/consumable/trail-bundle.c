inherit "/item/item";

string query_blueprint() { return "consumable/trail-bundle"; }

mapping item_profile() {
  return ([
    "name": "waxed bundle of trail rations",
    "alias": "rations",
    "description": "Dried fruit, soft cheese, and dense bread are tied up in waxed linen.",
    "slot": "none",
    "weight": 2,
    "value": 40,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 0,
    "travel_restore": 48
  ]);
}
