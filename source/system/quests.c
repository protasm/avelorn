/* Empty quest catalog retained as the stable boundary for future content. */
void initialize(mixed first_load) {
}

string title(string quest_id) { return "Unknown assignment"; }
string description(string quest_id) { return "No description is available."; }
int recommended_level(string quest_id) { return 1; }
int required_count(string quest_id) { return 1; }
int experience_reward(string quest_id) { return 0; }
int copper_reward(string quest_id) { return 0; }
string quest_for_defeat_tag(string tag) { return ""; }
string quest_for_action_tag(string tag) { return ""; }
int repeatable_defeat_tag(string tag) { return 0; }
string item_reward(string quest_id) { return ""; }
