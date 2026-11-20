int presentation_width = 80;
string pending_text;

void initialize(mixed first_load) {
  pending_text = "";
}

void write(mixed value) {
  string text;
  string line;
  int index;
  int start;

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
  jvmud_write(format_text(value));
}

void avelorn_write_to(object target, mixed value) {
  jvmud_write_to_lpc_object(target, complete_text(value));
}

void avelorn_emit_at(mixed place, mixed value) {
  jvmud_emit_perceivable_at(place, complete_text(value));
}

void avelorn_emit_except(mixed place, mixed value, mixed excluded) {
  jvmud_emit_perceivable_except(place, complete_text(value), excluded);
}

void flush_pending_text() {
  if (jvmud_size(pending_text) > 0) {
    jvmud_write(complete_text(pending_text));
    pending_text = "";
  }
}

void write_complete_line(string line) {
  jvmud_write(format_line(line) + "\n");
  if (is_room_heading(line)) {
    jvmud_write(room_ruler() + "\n");
  }
}

string complete_text(mixed value) {
  string output;
  int length;

  output = format_text(value);
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

string format_text(mixed value) {
  string *lines;
  string output;
  int index;

  lines = jvmud_split_text(normalize_newlines(jvmud_to_string(value)), "\n");
  output = format_line(lines[0]);
  for (index = 1; index < jvmud_size(lines); index += 1) {
    output += "\n" + format_line(lines[index]);
  }
  return output;
}

string format_line(string line) {
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
  if (length <= presentation_width) {
    return line;
  }

  start = 0;
  while (start < length && line[start] == ' ') {
    start += 1;
  }
  // Leave room for a character pair and a hyphen even with extreme indentation.
  indent_width = start < presentation_width - 3 ? start : presentation_width - 3;
  indent = indent_width ? jvmud_extract_text(line, 0, indent_width - 1) : "";
  width = presentation_width - indent_width;
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

string room_ruler() {
  string ruler;

  ruler = "";
  while (jvmud_size(ruler) < presentation_width) {
    ruler += "+=========";
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
