inherit "/item/item";

string query_blueprint() { return "armor/company-plate"; }

mapping item_profile() {
  return ([
    "name": "Ironhold company plate",
    "alias": "plate",
    "description": "Splinted steel over quilted cloth is joined with fittings a miner can replace.",
    "slot": "body",
    "weight": 16,
    "value": 430,
    "level": 8,
    "stat": "strength",
    "minimum": 15,
    "protection": 7
  ]);
}
