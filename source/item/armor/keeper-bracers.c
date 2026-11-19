inherit "/item/item";

string query_blueprint() { return "armor/keeper-bracers"; }

mapping item_profile() {
  return ([
    "name": "keeper's bracers",
    "alias": "bracers",
    "description": "Double strips over the forearms turn a knife away from the hands.",
    "slot": "hands",
    "weight": 2,
    "value": 135,
    "level": 4,
    "stat": "dexterity",
    "minimum": 12,
    "protection": 2
  ]);
}
