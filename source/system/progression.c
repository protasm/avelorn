// Each row gives experience needed to reach the level and training points earned there.
nosave mixed *levels;

void initialize(mixed first_load) {
  levels = ({
    ({   0, 0 }),  // 1
    ({ 100, 1 }),  // 2
    ({ 200, 0 }),  // 3
    ({ 300, 1 }),  // 4
    ({ 400, 0 }),  // 5
    ({ 500, 1 }),  // 6
    ({ 600, 0 }),  // 7
    ({ 700, 1 }),  // 8
    ({ 800, 0 }),  // 9
    ({ 900, 1 })   // 10
  });
}

int cap() {
  return jvmud_size(levels);
}

int experience_to_advance(int level) {
  return level >= 1 && level < cap() ? levels[level][0] : 0;
}

int training_points_for(int attained_level) {
  return attained_level >= 1 && attained_level <= cap() ? levels[attained_level - 1][1] : 0;
}
