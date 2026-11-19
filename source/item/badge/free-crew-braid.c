inherit "/item/item";

string query_blueprint() { return "badge/free-crew-braid"; }

mapping item_profile() {
  return ([
    "name": "free crew braid",
    "alias": "crew-braid",
    "description": "The red levy cord has been unpicked and braided together with lengths chosen by the crews who reclaimed their goods.",
    "slot": "badge",
    "weight": 0,
    "value": 0,
    "level": 1,
    "stat": "none",
    "minimum": 0
  ]);
}
