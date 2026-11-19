inherit "/item/item";

string query_blueprint() { return "weapon/concord-mace"; }

mapping item_profile() {
  return ([
    "name": "concord-faced mace",
    "alias": "concord-mace",
    "description": "One low ridge of the head carries metal worked by willing hands at a shared fire.",
    "slot": "weapon",
    "weight": 6,
    "value": 420,
    "level": 8,
    "stat": "wisdom",
    "minimum": 15,
    "power": 7,
    "damage_type": "crush",
    "concord": 1
  ]);
}
