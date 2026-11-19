inherit "/item/item";

string query_blueprint() { return "weapon/lakeward-staff"; }

mapping item_profile() {
  return ([
    "name": "Lakeward ash staff",
    "alias": "ash-staff",
    "description": "Two heat-blue rings hold a piece of water-clear volcanic glass in place.",
    "slot": "weapon",
    "weight": 6,
    "value": 420,
    "level": 8,
    "stat": "intelligence",
    "minimum": 16,
    "power": 8,
    "damage_type": "arcane"
  ]);
}
