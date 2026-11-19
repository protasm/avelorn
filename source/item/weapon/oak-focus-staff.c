inherit "/item/item";

string query_blueprint() { return "weapon/oak-focus-staff"; }

mapping item_profile() {
  return ([
    "name": "oak focus staff",
    "alias": "staff",
    "description": "Silver wire winds around a cut crystal at the staff's head.",
    "slot": "weapon",
    "weight": 5,
    "value": 150,
    "level": 1,
    "stat": "intelligence",
    "minimum": 11,
    "power": 3,
    "damage_type": "arcane"
  ]);
}
