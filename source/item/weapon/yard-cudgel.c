inherit "/item/item";

string query_blueprint() { return "weapon/yard-cudgel"; }

mapping item_profile() {
  return ([
    "name": "cooper's cudgel",
    "alias": "cudgel",
    "description": "Hoop iron binds the end of a short, heavy oak billet.",
    "slot": "weapon",
    "weight": 4,
    "value": 95,
    "level": 1,
    "stat": "strength",
    "minimum": 10,
    "power": 3,
    "damage_type": "crush"
  ]);
}
