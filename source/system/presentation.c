string pending_text;

void initialize(mixed first_load) {
  pending_text = "";
}

void write(mixed value) {
  string text;
  string line;
  int index;
  int start;

  text = pending_text + jvmud_to_string(value);
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
      write_complete_line(line);
      start = index + 1;
    }
    index += 1;
  }
  if (start < jvmud_size(text)) {
    pending_text = jvmud_extract_text(text, start);
  }
}

void avelorn_prompt(mixed value) {
  flush_pending_text();
  jvmud_write(value);
}

void avelorn_write_to(object target, mixed value) {
  jvmud_write_to_lpc_object(target, jvmud_wrap_text(value, 80));
}

void avelorn_emit_at(mixed place, mixed value) {
  jvmud_emit_perceivable_at(place, jvmud_wrap_text(value, 80));
}

void avelorn_emit_except(mixed place, mixed value, mixed excluded) {
  jvmud_emit_perceivable_except(place, jvmud_wrap_text(value, 80), excluded);
}

void flush_pending_text() {
  if (jvmud_size(pending_text) > 0) {
    jvmud_write(jvmud_wrap_text(pending_text, 80));
    pending_text = "";
  }
}

void write_complete_line(string line) {
  string prefix;
  string *lines;
  string output;
  int index;
  int width;

  prefix = "";
  index = 0;
  while (index < jvmud_size(line) && line[index] == ' ') {
    prefix += " ";
    index += 1;
  }
  if (index == jvmud_size(line)) {
    jvmud_write("\n");
  } else {
    width = 80 - index;
    lines = jvmud_split_text(jvmud_wrap_text(jvmud_extract_text(line, index), width), "\n");
    balance_last_line(lines, width);
    output = "";
    index = 0;
    // The wrapped text ends with a newline, so the final split field is empty.
    while (index < jvmud_size(lines) - 1) {
      if (jvmud_size(lines[index])) {
        output += prefix + lines[index];
      }
      output += "\n";
      index += 1;
    }
    if (jvmud_size(output)) {
      jvmud_write(output);
    }
  }
  if (is_room_heading(line)) {
    jvmud_write(room_ruler() + "\n");
  }
}

void balance_last_line(string *lines, int width) {
  string previous;
  int last;
  int split;

  last = jvmud_size(lines) - 2;
  if (last < 1 || jvmud_size(lines[last]) >= width / 4) {
    return;
  }
  // Give a short continuation a fuller line by breaking the last pair near its midpoint.
  previous = lines[last - 1];
  split = (jvmud_size(previous) + 1 + jvmud_size(lines[last])) / 2;
  while (split < jvmud_size(previous) && previous[split] != ' ') {
    split += 1;
  }
  if (split < jvmud_size(previous)) {
    lines[last] = jvmud_extract_text(previous, split + 1) + " " + lines[last];
    lines[last - 1] = jvmud_extract_text(previous, 0, split - 1);
  }
}

string room_ruler() {
  string ruler;
  string segment;
  int repeats;

  segment = "+=========";
  ruler = "";
  repeats = 0;
  while (repeats < 8) {
    ruler += segment;
    repeats += 1;
  }
  return ruler;
}

int is_room_heading(string line) {
  object actor;
  object place;

  actor = jvmud_current_actor();
  if (!actor) {
    return 0;
  }
  place = jvmud_entity_location(actor);
  if (!place || !jvmud_method_exists("short", place)) {
    return 0;
  }
  if (line == jvmud_invoke_lpc_object(place, "short")) {
    return 1;
  }
  return jvmud_method_exists("query_brief_short", place)
      && line == jvmud_invoke_lpc_object(place, "query_brief_short");
}
