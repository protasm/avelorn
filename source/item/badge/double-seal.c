inherit "/item/item";

string query_blueprint() { return "badge/double-seal"; }

mapping item_profile() {
  return ([
    "name": "double-witness clasp",
    "alias": "witness-clasp",
    "description": "Two impressions sit side by side on the clasp, one from the old archive and one from the people who kept its records dry.",
    "slot": "badge",
    "weight": 0,
    "value": 0,
    "level": 1,
    "stat": "none",
    "minimum": 0
  ]);
}
