#include "command_messages.h"

// Capturing in a shared service keeps authenticated Personas visible to JVMud's users().
nosave mapping clients;

void initialize(mixed first_load) {
  clients = ([ ]);
}

void prompt(object actor) {
  if (clients[actor] && jvmud_interactive(actor)) {
    jvmud_write_to_lpc_object(actor, "> ");
  }
}

void begin(object actor) {
  clients[actor] = ([
      "lines": ({ }),
      "overflow": 0
  ]);
  jvmud_capture_session_input("receive_line", 0);
  prompt(actor);
}

void end(object actor) {
  jvmud_mapping_delete(clients, actor);
}

void clear(object actor) {
  mapping client;

  client = clients[actor];
  if (client) {
    client["lines"] = ({ });
    client["overflow"] = 0;
  }
}

int count(object actor) {
  mapping client;

  client = clients[actor];
  if (client) {
    return jvmud_size(client["lines"]);
  }
  return 0;
}

string trim_line(string line) {
  int first;
  int last;

  first = 0;
  last = jvmud_size(line) - 1;
  while (first <= last && line[first] <= ' ') {
    first += 1;
  }
  while (last >= first && line[last] <= ' ') {
    last -= 1;
  }
  if (first > last) {
    return "";
  }
  return jvmud_extract_text(line, first, last);
}

void receive_line(mixed value) {
  object actor;
  mapping client;
  string line;
  string control;

  actor = jvmud_current_actor();
  client = clients[actor];
  if (!client || !jvmud_interactive(actor)) {
    return;
  }
  jvmud_capture_session_input("receive_line", 0);
  line = jvmud_to_string(value);
  if (jvmud_size(line) > 512) {
    if (!client["overflow"]) {
      client["overflow"] = 1;
      avelorn_write_to(actor, "Commands can contain up to 512 characters.\n");
    }
    return;
  }

  line = trim_line(line);
  control = jvmud_lowercase_text(line);
  if (control == "stop") {
    jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
    clear(actor);
    jvmud_invoke_lpc_object("system/travel", "clear", actor);
    avelorn_write_to(actor, "You stop and clear your queued commands.\n");
    prompt(actor);
    return;
  }
  if (control == "quit") {
    clear(actor);
    jvmud_invoke_lpc_object("system/travel", "clear", actor);
    jvmud_dispatch_player_command(actor, "quit");
    return;
  }
  if (!jvmud_size(line)) {
    prompt(actor);
    return;
  }
  if (jvmud_size(client["lines"]) >= 16) {
    if (!client["overflow"]) {
      client["overflow"] = 1;
      avelorn_write_to(actor, "Command queue full. Type stop to clear it.\n");
    }
    return;
  }
  client["lines"] = client["lines"] + ({ line });
}

int notify_unhandled_movement() {
  if (jvmud_member(({
      "north", "n", "east", "e", "south", "s", "west", "w",
      "northeast", "ne", "southeast", "se", "southwest", "sw", "northwest", "nw",
      "up", "u", "down", "d", "in", "enter", "out"
  }), jvmud_current_verb()) >= 0) {
    return jvmud_notify_fail(complete_text(
        INVALID_MOVEMENT_MESSAGE, display_width_for(jvmud_current_actor())));
  }
  return 0;
}

int drain(object actor) {
  mapping client;
  mixed *lines;
  string line;
  int index;

  client = clients[actor];
  if (!client || !jvmud_interactive(actor)) {
    return 0;
  }
  client["overflow"] = 0;
  for (index = 0; index < 6 && clients[actor] && jvmud_interactive(actor); index++) {
    lines = client["lines"];
    if (!jvmud_size(lines)) {
      break;
    }
    line = lines[0];
    if (jvmud_size(lines) > 1) {
      client["lines"] = lines[1..];
    } else {
      client["lines"] = ({ });
    }
    if (catch(jvmud_dispatch_player_command(actor, line, #'notify_unhandled_movement))) {
      avelorn_write_to(actor, "That command could not be completed.\n");
    }
  }
  return index > 0;
}
