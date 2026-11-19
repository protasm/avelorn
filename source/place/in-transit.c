string short() { return "Under Way"; }
string query_area() { return "Avelorn Transit"; }
string query_environment() { return "transit"; }
int query_gmcp_id() { return 29999999; }
mapping query_gmcp_exits() { return ([ ]); }
int query_safe() { return 1; }
int query_wilderness() { return 0; }

void describe(object viewer) {
  string status;

  status = jvmud_invoke_lpc_object("system/transit", "status", viewer);
  write(short() + "\n");
  if (jvmud_size(status)) {
    write(status + "\n");
  }
  write("Exits: none while traveling\n");
}

void describe_brief(object viewer) {
  describe(viewer);
}
