inherit "/item/item";

string query_blueprint() { return "weapon/netters-knife"; }

mapping item_profile() {
  return ([
    "name": "netter's long knife",
    "alias": "knife",
    "description": "The salt-dark blade is broad enough to part a fouled mooring line.",
    "slot": "weapon",
    "weight": 3,
    "value": 90,
    "level": 1,
    "stat": "dexterity",
    "minimum": 10,
    "power": 2,
    "damage_type": "cut"
  ]);
}
