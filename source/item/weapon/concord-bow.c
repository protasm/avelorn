inherit "/item/item";

string query_blueprint() { return "weapon/concord-bow"; }

mapping item_profile() {
  return ([
    "name": "concord-tipped bow kit",
    "alias": "concord-bow",
    "description": "The ash bow carries a field quiver fitted with the shared hearth's concord points.",
    "slot": "weapon",
    "weight": 5,
    "value": 420,
    "level": 8,
    "stat": "dexterity",
    "minimum": 15,
    "power": 7,
    "damage_type": "pierce",
    "concord": 1
  ]);
}
