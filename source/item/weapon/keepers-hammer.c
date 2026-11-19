inherit "/item/item";

string query_blueprint() { return "weapon/keepers-hammer"; }

mapping item_profile() {
  return ([
    "name": "keeping-house hammer",
    "alias": "hammer",
    "description": "The small iron face bears the mark used to certify repairs made in public trust.",
    "slot": "weapon",
    "weight": 4,
    "value": 220,
    "level": 3,
    "stat": "wisdom",
    "minimum": 13,
    "power": 5,
    "damage_type": "crush"
  ]);
}
