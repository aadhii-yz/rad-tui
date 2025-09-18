#define RUN_TESTS
#include "game.c"

// Tests
// TODO: Move to seperate test file
// TODO: Add more tests

int test_player_lives() {
  GameState state = {
      .old_screen = {{'X', 'X', 'X', 'X', '\n'},
                     {'X', 'O', 'O', 'X', '\n'},
                     {'X', '@', '.', 'X', '\n'},
                     {'X', 'X', 'X', 'X', '\n'}},
      .key = 3,
      .pos_x = 1,
      .pos_y = 2,
  };

  char expected1[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', 'o', 'O', 'X', '\n'},
                          {'X', ' ', '@', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  char expected2[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', ' ', 'O', 'X', '\n'},
                          {'X', 'o', '@', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  char expected3[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', ' ', 'O', 'X', '\n'},
                          {'X', 'O', '@', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  update(&state);
  int ret = memcmp(state.screen, expected1, sizeof(expected1));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected1\n");
    // memset(state.old_screen, '?', sizeof(state.old_screen));
    // render(&state);
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));
  state.key = 0;

  update(&state);
  ret = memcmp(state.screen, expected2, sizeof(expected2));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected2\n");
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));

  update(&state);
  ret = memcmp(state.screen, expected3, sizeof(expected3));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected3\n");
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));

  return 0;
}

int test_rock_rolls() {
  GameState state = {
      .old_screen = {{'X', 'X', 'X', 'X', '\n'},
                     {'X', ' ', 'O', 'X', '\n'},
                     {'X', ' ', 'O', 'X', '\n'},
                     {'X', 'X', 'X', 'X', '\n'}},
      .key = 0,
      .pos_x = 0,
      .pos_y = 0,
  };

  char expected1[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', ' ', 'o', 'X', '\n'},
                          {'X', ' ', 'O', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  char expected2[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', 'o', ' ', 'X', '\n'},
                          {'X', ' ', 'O', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  char expected3[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', ' ', ' ', 'X', '\n'},
                          {'X', 'o', 'O', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  char expected4[4][5] = {{'X', 'X', 'X', 'X', '\n'},
                          {'X', ' ', ' ', 'X', '\n'},
                          {'X', 'O', 'O', 'X', '\n'},
                          {'X', 'X', 'X', 'X', '\n'}};

  update(&state);
  int ret = memcmp(state.screen, expected1, sizeof(expected1));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected1\n");
    // memset(state.old_screen, '?', sizeof(state.old_screen));
    // render(&state);
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));

  update(&state);
  ret = memcmp(state.screen, expected2, sizeof(expected2));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected2\n");
    memset(state.old_screen, '?', sizeof(state.old_screen));
    render(&state);
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));

  update(&state);
  ret = memcmp(state.screen, expected3, sizeof(expected3));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected3\n");
    return 1;
  }
  memcpy(state.old_screen, state.screen, sizeof(state.screen));

  update(&state);
  ret = memcmp(state.screen, expected4, sizeof(expected4));
  if (ret != 0) {
    printf(
        "\e[38;2;250;10;10mState after update() is not equal to expected4\n");
    return 1;
  }

  return 0;
}

int main() {
  int tests = 0;
  int evaluation = 0;

  int ret = test_rock_rolls();
  if (ret != 0) {
    printf("\e[38;2;250;10;10mTest Rock Falls - Failed\n");
  } else {
    printf("\e[38;2;10;250;10mTest Rock Falls - Successful\n");
  }
  evaluation += ret;
  ++tests;

  ret = test_player_lives();
  if (ret != 0) {
    printf("\e[38;2;250;10;10mTest Player Lives - Failed\n");
  } else {
    printf("\e[38;2;10;250;10mTest Player Lives - Successful\n");
  }
  evaluation += ret;
  ++tests;

  if (evaluation == 0) {
    printf("\e[38;2;10;250;10mALL %d test were Successful\n", tests);
  } else {
    printf("\e[38;2;250;10;10mTests Failed\n");
  }

  printf("\e[m");
}

