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
  jvmud_write(format_text(value, columns));
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
  jvmud_write(format_line(line, columns) + "\n");
  if (is_room_heading(line, viewer)) {
    jvmud_write(room_ruler(columns) + "\n");
  }
}

string complete_text(mixed value, int columns) {
  string output;
  int length;

  output = format_text(value, columns);
  length = jvmud_size(output);
  if (length && output[length - 1] != '\n') {
    output += "\n";
  }
  return output;
}

string normalize_newlines(string text) {
  if (jvmud_member(text, '\r') >= 0) {
    return jvmud_regex_replace(text, "\r\n?", "\n", 1);
  }
  return text;
}

string format_text(mixed value, int columns) {
  string *lines;
  string output;
  int index;

  lines = jvmud_split_text(normalize_newlines(jvmud_to_string(value)), "\n");
  output = format_line(lines[0], columns);
  for (index = 1; index < jvmud_size(lines); index += 1) {
    output += "\n" + format_line(lines[index], columns);
  }
  return output;
}

string format_line(string line, int columns) {
  string indent;
  string output;
  string hyphen;
  int length;
  int indent_width;
  int width;
  int start;
  int split;
  int next;

  if (jvmud_member(line, '\t') >= 0 || jvmud_member(line, "\\t") >= 0) {
    line = jvmud_regex_replace(line, "\\\\t|\t", "    ", 1);
  }
  length = jvmud_size(line);
  if (length <= columns) {
    return line;
  }

  start = 0;
  while (start < length && line[start] == ' ') {
    start += 1;
  }
  // Leave room for a character pair and a hyphen even with extreme indentation.
  indent_width = start < columns - 3 ? start : columns - 3;
  indent = indent_width ? jvmud_extract_text(line, 0, indent_width - 1) : "";
  width = columns - indent_width;
  output = "";
  while (length - start > width) {
    split = start + width;
    while (split > start && line[split] != ' '
        && (split <= start + 1 || (line[split - 1] != '-' && line[split - 1] != '/'))) {
      split -= 1;
    }
    hyphen = "";
    if (split == start) {
      split = start + width - 1;
      // A forced split must not bisect a UTF-16 surrogate pair.
      if (line[split] >= 56320 && line[split] <= 57343) {
        split -= 1;
      }
      next = split;
      hyphen = "-";
    } else {
      next = split;
      while (next < length && line[next] == ' ') {
        next += 1;
      }
      while (line[split - 1] == ' ') {
        split -= 1;
      }
    }
    output += indent + jvmud_extract_text(line, start, split - 1) + hyphen;
    start = next;
    if (start < length) {
      output += "\n";
    }
  }
  if (start < length) {
    output += indent + jvmud_extract_text(line, start);
  }
  return output;
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
