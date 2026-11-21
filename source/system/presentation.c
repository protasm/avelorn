string pending_text;

void initialize(mixed first_load) {
  pending_text = "";
}

int valid_display_width(int columns) {
  return columns >= 20 && columns <= 120;
}

int display_width_for(object viewer) {
  int columns;

  if (viewer && jvmud_method_exists("query_display_width", viewer)) {
    columns = jvmud_invoke_lpc_object(viewer, "query_display_width");
    if (valid_display_width(columns)) {
      return columns;
    }
  }
  return 60;
}

void write(mixed value) {
  object viewer;
  string text;
  string line;
  int columns;
  int index;
  int start;

  viewer = jvmud_current_actor();
  columns = display_width_for(viewer);
  text = pending_text + normalize_newlines(jvmud_to_string(value));
  pending_text = "";
  start = 0;
  index = 0;
  while (index < jvmud_size(text)) {
    if (text[index] == '\n') {
      if (index > start) {
        line = jvmud_extract_text(text, start, index - 1);
      } else {
        line = "";
      }
      write_complete_line(line, viewer, columns);
      start = index + 1;
    }
    index += 1;
  }
  if (start < jvmud_size(text)) {
    pending_text = jvmud_extract_text(text, start);
  }
}

void avelorn_prompt(mixed value) {
  int columns;

  columns = display_width_for(jvmud_current_actor());
  if (jvmud_size(pending_text) > 0) {
    jvmud_write(complete_text(pending_text, columns));
    pending_text = "";
  }
  jvmud_write(format_output(value, columns));
}

void avelorn_write_to(object target, mixed value) {
  jvmud_write_to_lpc_object(target, complete_text(value, display_width_for(target)));
}

void avelorn_emit_at(object place, mixed value) {
  emit_to_room(place, value, 0, 0);
}

void avelorn_emit_except(object source, mixed value, object excluded) {
  emit_to_room(jvmud_entity_location(source), value, source, excluded);
}

void emit_to_room(object place, mixed value, object source, object excluded) {
  object viewer;

  if (!place) {
    return;
  }
  // Wrap the same message independently for each connected viewer in the room.
  viewer = jvmud_first_entity_at(place);
  while (viewer) {
    if (viewer != source && viewer != excluded && jvmud_interactive(viewer)) {
      avelorn_write_to(viewer, value);
    }
    viewer = jvmud_next_entity_at(viewer);
  }
}

void write_complete_line(string line, object viewer, int columns) {
  jvmud_write(format_output(line, columns) + "\n");
  if (is_room_heading(line, viewer)) {
    jvmud_write(room_ruler(columns) + "\n");
  }
}

string complete_text(mixed value, int columns) {
  string output;
  int length;

  output = format_output(value, columns);
  length = jvmud_size(output);
  if (length && output[length - 1] != '\n') {
    output += "\n";
  }
  return output;
}

string format_output(mixed value, int columns) {
  string text;

  text = jvmud_to_string(value);
  // Expand escaped tabs before the driver measures the text.
  if (jvmud_member(text, "\\t") >= 0) {
    text = jvmud_regex_replace(text, "\\\\t", "    ", 1);
  }
  return jvmud_wrap_text(text, columns, 1);
}

string normalize_newlines(string text) {
  if (jvmud_member(text, '\r') >= 0) {
    return jvmud_regex_replace(text, "\r\n?", "\n", 1);
  }
  return text;
}

string room_ruler(int columns) {
  string ruler;

  ruler = "";
  while (jvmud_size(ruler) < columns) {
    ruler += "+=========";
  }
  return jvmud_extract_text(ruler, 0, columns - 1);
}

int is_room_heading(string line, object viewer) {
  object place;

  if (!viewer) {
    return 0;
  }
  place = jvmud_entity_location(viewer);
  if (!place || !jvmud_method_exists("short", place)) {
    return 0;
  }
  if (line == jvmud_invoke_lpc_object(place, "short")) {
    return 1;
  }
  return jvmud_method_exists("query_brief_short", place)
      && line == jvmud_invoke_lpc_object(place, "query_brief_short");
}
