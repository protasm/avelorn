inherit "/item/item";

string query_blueprint() { return "armor/hearing-coat"; }

mapping item_profile() {
  return ([
    "name": "reinforced hearing coat",
    "alias": "coat",
    "description": "Close links are hidden inside the civilian coat used at district hearings.",
    "slot": "body",
    "weight": 7,
    "value": 225,
    "level": 4,
    "stat": "constitution",
    "minimum": 12,
    "protection": 4
  ]);
}
