void initialize(mixed first_load) {
}

object compile_atlas_room(string path) {
  mixed square;
  object room;

  square = jvmud_invoke_lpc_object("system/atlas", "square_for_path", path);
  if (!square) {
    return 0;
  }
  room = jvmud_clone_lpc_object("system/atlas_room");
  jvmud_invoke_lpc_object(
      room, "configure", square[0], square[1], square[2], square[3], square[4]);
  return room;
}
