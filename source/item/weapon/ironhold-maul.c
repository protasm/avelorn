inherit "/item/item";

string query_blueprint() { return "weapon/ironhold-maul"; }

mapping item_profile() {
  return ([
    "name": "Ironhold sledge",
    "alias": "maul",
    "description": "A bound company's stamp marks the heel of this heavy square head.",
    "slot": "weapon",
    "weight": 10,
    "value": 425,
    "level": 8,
    "stat": "strength",
    "minimum": 16,
    "power": 8,
    "damage_type": "crush"
  ]);
}
