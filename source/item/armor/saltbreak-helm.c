inherit "/item/item";

string query_blueprint() { return "armor/saltbreak-helm"; }

mapping item_profile() {
  return ([
    "name": "Saltbreak steel helm",
    "alias": "helm",
    "description": "A short brow guards the eyes without catching the wind.",
    "slot": "head",
    "weight": 4,
    "value": 190,
    "level": 5,
    "stat": "strength",
    "minimum": 12,
    "protection": 3
  ]);
}
