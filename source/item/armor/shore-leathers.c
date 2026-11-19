inherit "/item/item";

string query_blueprint() { return "armor/shore-leathers"; }

mapping item_profile() {
  return ([
    "name": "salt-cured shore leathers",
    "alias": "leathers",
    "description": "The seams have been cut away from the shoulders for boarding and climbing.",
    "slot": "body",
    "weight": 5,
    "value": 175,
    "level": 3,
    "stat": "dexterity",
    "minimum": 12,
    "protection": 3
  ]);
}
