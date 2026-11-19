inherit "/item/item";

string query_blueprint() { return "badge/open-roof-pin"; }

mapping item_profile() {
  return ([
    "name": "open-roof pin",
    "alias": "roof-pin",
    "description": "A small blue tile set in tin marks a place kept open for a guest whose shelter debt was canceled.",
    "slot": "badge",
    "weight": 0,
    "value": 0,
    "level": 1,
    "stat": "none",
    "minimum": 0
  ]);
}
