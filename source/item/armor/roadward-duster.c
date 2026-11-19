inherit "/item/item";

string query_blueprint() { return "armor/roadward-duster"; }

mapping item_profile() {
  return ([
    "name": "roadward duster",
    "alias": "duster",
    "description": "Fine flexible scales keep weight off the shoulders without leaving the ribs bare.",
    "slot": "body",
    "weight": 6,
    "value": 425,
    "level": 8,
    "stat": "dexterity",
    "minimum": 14,
    "protection": 5
  ]);
}
