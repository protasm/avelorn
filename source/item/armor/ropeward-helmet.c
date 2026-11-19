inherit "/item/item";

string query_blueprint() { return "armor/ropeward-helmet"; }

mapping item_profile() {
  return ([
    "name": "ropeward helmet",
    "alias": "helmet",
    "description": "The oil-cured lining can be washed after days of dust and smoke.",
    "slot": "head",
    "weight": 5,
    "value": 245,
    "level": 8,
    "stat": "constitution",
    "minimum": 14,
    "protection": 4
  ]);
}
