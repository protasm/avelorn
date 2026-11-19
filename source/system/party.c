// Party membership, invitations, and following are scoped to live sessions.
nosave mapping parties;
nosave mapping invitations;
nosave mapping following;

void initialize(mixed first_load) {
  if (!parties) {
    parties = ([ ]);
    invitations = ([ ]);
    following = ([ ]);
  }
}

object leader(object actor) {
  mapping party;

  party = parties[actor];
  return party ? party["leader"] : 0;
}

mixed *roster(object actor) {
  mapping party;

  party = parties[actor];
  return party ? party["members"] + ({ }) : ({ actor });
}

string name(object actor) {
  return jvmud_invoke_lpc_object(actor, "query_name");
}

void tell(object actor, string message) {
  jvmud_write_to_lpc_object(actor, message + "\n");
}

void announce(mixed *members, string message) {
  int index;

  for (index = 0; index < jvmud_size(members); index++) {
    tell(members[index], message);
  }
}

int same_place(object first, object second) {
  object place;

  place = jvmud_entity_location(first);
  return place && place == jvmud_entity_location(second);
}

object nearby_named(object actor, mixed *candidates, string requested) {
  object candidate;
  object match;
  int index;

  for (index = 0; index < jvmud_size(candidates); index++) {
    candidate = candidates[index];
    if (candidate != actor && same_place(actor, candidate)
        && jvmud_method_exists("query_name", candidate)
        && jvmud_lowercase_text(name(candidate)) == requested) {
      if (match) { return 0; }
      match = candidate;
    }
  }
  return match;
}

object nearby_member(object actor, string requested) {
  return nearby_named(actor, roster(actor), requested);
}

int status(object actor) {
  mapping party;
  mixed *members;
  string text;
  int index;

  party = parties[actor];
  if (!party) {
    tell(actor, "You are traveling alone. Use party invite <name> to invite someone nearby.");
    return 1;
  }
  members = party["members"];
  text = "Party led by " + name(party["leader"]) + " (" + jvmud_size(members) + "/4):";
  for (index = 0; index < jvmud_size(members); index++) {
    text += "\n  " + name(members[index]);
    if (following[members[index]]) {
      text += " (following)";
    }
  }
  tell(actor, text);
  return 1;
}

int invite(object actor, string requested) {
  object target;
  mapping pending;

  if (leader(actor) && leader(actor) != actor) {
    tell(actor, "Only the party leader may invite adventurers.");
    return 1;
  }
  if (jvmud_size(roster(actor)) >= 4) {
    tell(actor, "Your party is full.");
    return 1;
  }
  target = nearby_named(actor, jvmud_users(), requested);
  if (!target) {
    tell(actor, "No single nearby adventurer matches that name.");
    return 1;
  }
  if (parties[target]) {
    tell(actor, name(target) + " is already in a party.");
    return 1;
  }
  pending = invitations[target];
  if (pending && pending["expires"] > jvmud_time()) {
    tell(actor, name(target) + " already has a party invitation.");
    return 1;
  }
  invitations[target] = ([ "from": actor, "expires": jvmud_time() + 60 ]);
  tell(actor, "You invite " + name(target) + " to your party.");
  tell(target, name(actor) + " invites you to join a party. Use party accept "
      + name(actor) + " or party decline.");
  return 1;
}

int answer(object actor, string requested, int accept) {
  mapping pending;
  mapping party;
  object inviter;
  mixed *members;

  pending = invitations[actor];
  if (!pending || pending["expires"] <= jvmud_time()) {
    invitations = jvmud_mapping_delete(invitations, actor);
    tell(actor, "You have no current party invitation.");
    return 1;
  }
  inviter = pending["from"];
  if (jvmud_size(requested) && jvmud_lowercase_text(name(inviter)) != requested) {
    tell(actor, "That adventurer did not send your current invitation.");
    return 1;
  }
  if (!accept) {
    invitations = jvmud_mapping_delete(invitations, actor);
    tell(actor, "You decline the party invitation.");
    tell(inviter, name(actor) + " declines your party invitation.");
    return 1;
  }
  if (!same_place(actor, inviter)) {
    tell(actor, "Rejoin " + name(inviter) + " in person before accepting.");
    return 1;
  }
  invitations = jvmud_mapping_delete(invitations, actor);
  party = parties[inviter];
  if (parties[actor] || (party && party["leader"] != inviter)) {
    tell(actor, "That party invitation is no longer available.");
    return 1;
  }
  members = roster(inviter);
  if (jvmud_size(members) >= 4) {
    tell(actor, "That party is already full.");
    return 1;
  }
  if (!party) {
    party = ([ "leader": inviter ]);
    parties[inviter] = party;
  }
  party["members"] = members + ({ actor });
  parties[actor] = party;
  announce(party["members"], name(actor) + " joins the party.");
  return 1;
}

void stop_following(object actor, string message) {
  if (following[actor]) {
    following = jvmud_mapping_delete(following, actor);
    jvmud_invoke_lpc_object("system/travel", "clear_follow", actor);
    if (jvmud_size(message)) { tell(actor, message); }
  }
}

int follow(object actor, string option) {
  object guide;

  if (option == "off") {
    stop_following(actor, "");
    tell(actor, "Party following is off.");
    return 1;
  }
  if (option != "on") {
    tell(actor, "Use party follow on or party follow off.");
    return 1;
  }
  guide = leader(actor);
  if (!guide || guide == actor || !same_place(actor, guide)
      || jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    tell(actor, "Stand near your party leader before following.");
    return 1;
  }
  if (jvmud_invoke_lpc_object("system/travel", "count", actor)) {
    tell(actor, "Finish your queued movements or type stop before following.");
    return 1;
  }
  following[actor] = guide;
  tell(actor, "You begin following " + name(guide) + ".");
  return 1;
}

void stop_followers(object actor) {
  mixed *members;
  int index;

  members = roster(actor);
  for (index = 0; index < jvmud_size(members); index++) {
    if (following[members[index]] == actor) {
      stop_following(members[index], "You stop following " + name(actor) + ".");
    }
  }
}

void schedule_followers(object guide, string origin, string direction, string destination) {
  mixed *members;
  object follower;
  int index;

  if (leader(guide) != guide) {
    return;
  }
  members = roster(guide);
  for (index = 0; index < jvmud_size(members); index++) {
    follower = members[index];
    if (following[follower] == guide && !jvmud_invoke_lpc_object(
        "system/travel", "enqueue_follow", follower, origin, direction, destination)) {
      stop_following(follower, "You lose " + name(guide) + "'s trail. Regroup before following again.");
    }
  }
}

void detach(object actor) {
  mapping party;
  mixed *members;
  mixed *invitees;
  mixed *remaining;
  int index;

  stop_following(actor, "");
  stop_followers(actor);
  invitees = jvmud_mapping_keys(invitations);
  for (index = 0; index < jvmud_size(invitees); index++) {
    if (invitees[index] == actor || invitations[invitees[index]]["from"] == actor) {
      invitations = jvmud_mapping_delete(invitations, invitees[index]);
    }
  }
  party = parties[actor];
  if (!party) {
    return;
  }
  parties = jvmud_mapping_delete(parties, actor);
  members = party["members"];
  remaining = ({ });
  for (index = 0; index < jvmud_size(members); index++) {
    if (members[index] != actor) {
      remaining += ({ members[index] });
    }
  }
  if (jvmud_size(remaining) < 2) {
    if (jvmud_size(remaining)) {
      parties = jvmud_mapping_delete(parties, remaining[0]);
    }
    announce(remaining, "The party disbands.");
    return;
  }
  party["members"] = remaining;
  announce(remaining, name(actor) + " leaves the party.");
  if (party["leader"] == actor) {
    party["leader"] = remaining[0];
    announce(remaining, name(remaining[0]) + " now leads the party.");
  }
}

int command(object actor, mixed argument) {
  string input;
  string verb;
  string detail;
  int end;

  input = argument ? jvmud_to_string(argument) : "";
  while (end < jvmud_size(input) && input[end] != ' ') {
    end += 1;
  }
  verb = end ? jvmud_lowercase_text(jvmud_extract_text(input, 0, end - 1)) : "";
  detail = end < jvmud_size(input)
      ? jvmud_invoke_lpc_object("system/input", "trim_line", jvmud_extract_text(input, end))
      : "";
  if (!jvmud_size(verb) || verb == "status") { return status(actor); }
  if (verb == "invite") { return invite(actor, jvmud_lowercase_text(detail)); }
  if (verb == "accept") { return answer(actor, jvmud_lowercase_text(detail), 1); }
  if (verb == "decline") { return answer(actor, jvmud_lowercase_text(detail), 0); }
  if (verb == "follow") { return follow(actor, jvmud_lowercase_text(detail)); }
  if (verb == "leave") {
    detach(actor);
    tell(actor, "You leave the party.");
    return 1;
  }
  if (verb == "say" && parties[actor] && jvmud_size(detail)) {
    announce(roster(actor), "[Party] " + name(actor) + ": " + detail);
    return 1;
  }
  tell(actor, "Use party, party invite <name>, party accept [name], party decline, "
      + "party follow on|off, party say <message>, or party leave.");
  return 1;
}
