// The live world retains a small set of reusable empty wilderness Places.
nosave mapping places;
nosave object *slots;
nosave mapping dropped;

void initialize(mixed first_load) {
  if (!places) {
    places = ([ ]);
    slots = ({ });
    dropped = ([ ]);
    jvmud_schedule_recurring_tick(1, 30);
  }
}

object acquire(string path, mixed *square) {
  object room;
  object candidate;
  int index;

  room = places[path];
  if (room) {
    return room;
  }
  for (index = 0; index < jvmud_size(slots); index += 1) {
    candidate = slots[index];
    if (!jvmud_first_entity_at(candidate)) {
      return bind(candidate, path, square);
    }
  }
  if (jvmud_size(slots) >= 256) {
    return 0;
  }
  room = jvmud_clone_lpc_object("system/atlas_room");
  slots += ({ room });
  return bind(room, path, square);
}

object bind(object room, string path, mixed *square) {
  string previous;

  previous = jvmud_invoke_lpc_object(room, "atlas_path");
  jvmud_mapping_delete(places, previous);
  jvmud_invoke_lpc_object(
      room, "configure", square[0], square[1], square[2], square[3], square[4]);
  places[path] = room;
  return room;
}

int room_count() {
  return jvmud_size(slots);
}

void note_drop(object item, object room) {
  if (room && jvmud_method_exists("atlas_path", room)) {
    dropped[item] = ({ room, jvmud_time() + 120 });
  }
}

void forget_drop(object item) {
  if (dropped[item]) {
    jvmud_mapping_delete(dropped, item);
  }
}

void scheduled_update() {
  mixed *items;
  mixed *entry;
  object item;
  int now;
  int index;

  items = jvmud_mapping_keys(dropped);
  now = jvmud_time();
  for (index = 0; index < jvmud_size(items); index += 1) {
    item = items[index];
    entry = dropped[item];
    if (!item || jvmud_entity_location(item) != entry[0]) {
      jvmud_mapping_delete(dropped, item);
    } else if (entry[1] <= now) {
      jvmud_mapping_delete(dropped, item);
      jvmud_destroy_lpc_object(item);
    }
  }
}
