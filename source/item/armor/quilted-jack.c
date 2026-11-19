inherit "/item/item";

string query_blueprint() { return "armor/quilted-jack"; }

mapping item_profile() {
  return ([
    "name": "quilted sailcloth jack",
    "alias": "jack",
    "description": "Old sail panels cover layers of padding stitched in narrow lines.",
    "slot": "body",
    "weight": 5,
    "value": 110,
    "level": 1,
    "stat": "constitution",
    "minimum": 10,
    "protection": 2
  ]);
}
