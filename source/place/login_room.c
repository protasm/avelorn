void initialize(mixed first_load) {
}

string short() {
  return "Login Room";
}

void describe(object viewer) {
  write("Login Room\n");
  write("This is Avelorn's temporary login room. The world beyond it has not ");
  write("been built yet. There are no exits.\n");
}

string query_area() {
  return "Login";
}

string query_environment() {
  return "room";
}

int query_gmcp_id() {
  return 1;
}

mapping query_gmcp_exits() {
  return ([ ]);
}
