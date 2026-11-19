inherit "/item/item";

string query_blueprint() { return "badge/shared-iron-rivet"; }

mapping item_profile() {
  return ([
    "name": "shared iron rivet",
    "alias": "company-rivet",
    "description": "Six old company names are struck around the head of an ordinary furnace rivet. The head is polished where a second hand steadied it.",
    "slot": "badge",
    "weight": 0,
    "value": 0,
    "level": 1,
    "stat": "none",
    "minimum": 0
  ]);
}
