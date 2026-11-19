inherit "/item/item";

string query_blueprint() { return "badge/red-run-knot"; }

mapping item_profile() {
  return ([
    "name": "red sluice knot",
    "alias": "sluice-knot",
    "description": "A ring of cord colored with the Red Run kiln's red clay. The spillway crew wears the knot at the end of a safe shift.",
    "slot": "badge",
    "weight": 0,
    "value": 0,
    "level": 1,
    "stat": "none",
    "minimum": 0
  ]);
}
