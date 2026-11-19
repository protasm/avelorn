inherit "/item/item";

string query_blueprint() { return "armor/chain-hauberk"; }

mapping item_profile() {
  return ([
    "name": "Freeharbor chain hauberk",
    "alias": "hauberk",
    "description": "A short skirt keeps the tightly closed rings out of the knees on a wet ladder.",
    "slot": "body",
    "weight": 12,
    "value": 345,
    "level": 6,
    "stat": "strength",
    "minimum": 13,
    "protection": 6
  ]);
}
