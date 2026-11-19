inherit "/item/item";

string query_blueprint() { return "consumable/field-bread"; }

mapping item_profile() {
  return ([
    "name": "loaf of field bread",
    "alias": "bread",
    "description": "A dense oat-and-honey loaf baked to travel well.",
    "slot": "none",
    "weight": 1,
    "value": 12,
    "level": 1,
    "stat": "none",
    "minimum": 0,
    "healing": 5,
    "travel_restore": 12
  ]);
}
