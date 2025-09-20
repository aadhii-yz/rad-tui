// Includes :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "game.h"

// Handle key press
int read_key(char *buf, int k) {
  if (buf[k] == '\033' && buf[k + 1] == '[') {
    switch (buf[k + 2]) {
    case 'A':
      return 1; // UP
    case 'B':
      return 2; // DOWN
    case 'C':
      return 3; // RIGHT
    case 'D':
      return 4; // LEFT
    }
  }
  return 0;
}

// Read key press
void read_input(GameState *state) {
  char buf[4096];
  int n = read(STDIN_FILENO, buf, sizeof(buf));
  int final_key = 0;
  for (int k = 0; k <= n - 3; k += 3) {
    int key = read_key(buf, k);
    if (key == 0)
      continue;
    final_key = key;
  }
  state->key = final_key;
}

// Update game state based on key perss
void handle_player(GameState *state) {
  switch (state->key) {
  case 1:
    switch (state->screen[state->pos_y - 1][state->pos_x]) {
    case '$':
      state->gems_collected++;
    case ' ':
    case '.':
      state->screen[state->pos_y][state->pos_x] = ' ';
      state->screen[state->pos_y - 1][state->pos_x] = '@';
      --state->pos_y;
      break;
    case 'E':
      state->won = 1;
      break;
    case 'o':
    case 'S':
      state->dead = 1;
      break;
    default:
      break;
    }
    break;
  case 2:
    switch (state->screen[state->pos_y + 1][state->pos_x]) {
    case '$':
      state->gems_collected++;
    case ' ':
    case '.':
      state->screen[state->pos_y][state->pos_x] = ' ';
      state->screen[state->pos_y + 1][state->pos_x] = '@';
      ++state->pos_y;
      break;
    case 'E':
      state->won = 1;
      break;
    default:
      break;
    }
    break;
  case 3:
    switch (state->screen[state->pos_y][state->pos_x + 1]) {
    case '$':
      state->gems_collected++;
    case ' ':
    case '.':
      state->screen[state->pos_y][state->pos_x] = ' ';
      state->screen[state->pos_y][state->pos_x + 1] = '@';
      ++state->pos_x;
      break;
    case 'E':
      state->won = 1;
      break;
    case 'O':
      if (state->screen[state->pos_y][state->pos_x + 2] == ' ') {
        state->screen[state->pos_y][state->pos_x + 2] = 'O';
        state->screen[state->pos_y][state->pos_x] = ' ';
        state->screen[state->pos_y][state->pos_x + 1] = '@';
        ++state->pos_x;
      }
      break;
    default:
      break;
    }
    break;
  case 4:
    switch (state->screen[state->pos_y][state->pos_x - 1]) {
    case '$':
      state->gems_collected++;
    case ' ':
    case '.':
      state->screen[state->pos_y][state->pos_x] = ' ';
      state->screen[state->pos_y][state->pos_x - 1] = '@';
      --state->pos_x;
      break;
    case 'E':
      state->won = 1;
      break;
    case 'O':
      if (state->screen[state->pos_y][state->pos_x - 2] == ' ') {
        state->screen[state->pos_y][state->pos_x - 2] = 'O';
        state->screen[state->pos_y][state->pos_x] = ' ';
        state->screen[state->pos_y][state->pos_x - 1] = '@';
        --state->pos_x;
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

// Checks if the rock or the gems are falling, then updates state.
void handle_rocks_gems(GameState *state, int x, int y) {
  int gem = state->screen[y][x] == '$';
  if (state->screen[y + 1][x] == ' ') {
    if (gem) {
      state->screen[y][x] = 'S';
    } else {
      state->screen[y][x] = 'o';
    }
    return;
  }
  if (state->screen[y + 1][x] == 'O' || state->screen[y + 1][x] == '$') {
    if (state->screen[y][x - 1] == ' ' && state->screen[y + 1][x - 1] == ' ') {
      if (gem) {
        state->screen[y][x] = 'S';
      } else {
        state->screen[y][x] = 'o';
      }
    }
    if (state->screen[y][x + 1] == ' ' && state->screen[y + 1][x + 1] == ' ') {
      if (gem) {
        state->screen[y][x] = 'S';
      } else {
        state->screen[y][x] = 'o';
      }
    }
  }
}

// Update the next state of the falling rocks and gems.
void handle_falling_rocks_gems(GameState *state, int x, int y) {
  int gem = state->screen[y][x] == 'S';
  if (state->screen[y + 1][x] == ' ') {
    state->screen[y][x] = ' ';
    if (gem) {
      state->screen[y + 1][x] = 'S';
    } else {
      state->screen[y + 1][x] = 'o';
    }
    return;
  }
  if (state->screen[y + 1][x] == 'O' || state->screen[y + 1][x] == '$') {
    if (state->screen[y][x - 1] == ' ' && state->screen[y + 1][x - 1] == ' ') {
      state->screen[y][x] = ' ';
      if (gem) {
        state->screen[y][x - 1] = 'p';
      } else {
        state->screen[y][x - 1] = 'i';
      }
      return;
    }
    if (state->screen[y][x + 1] == ' ' && state->screen[y + 1][x + 1] == ' ') {
      state->screen[y][x] = ' ';
      if (gem) {
        state->screen[y][x + 1] = 'S';
      } else {
        state->screen[y][x + 1] = 'o';
      }
      return;
    }
  }
  if (state->screen[y + 1][x] == 'o' || state->screen[y + 1][x] == 'S')
    return;
  if (state->screen[y + 1][x] == '@') {
    state->dead = 1;
    return;
  }
  if (gem) {
    state->screen[y][x] = '$';
  } else {
    state->screen[y][x] = 'O';
  }
}

// Handle all elements based on the current state
void update_all_elements(GameState *state) {
  for (int j = MAX_Y - 1; j != 0; --j) {
    for (int i = MAX_X - 2; i != 0; --i) {
      switch (state->screen[j][i]) {
      case 'p':
        state->screen[j][i] = 'S';
        break;
      case 'i':
        state->screen[j][i] = 'o';
        break;
      case 'O':
      case '$':
        handle_rocks_gems(state, i, j);
        break;
      case 'o':
      case 'S':
        handle_falling_rocks_gems(state, i, j);
        break;
      default:
        break;
      }
    }
  }
}

// Updates all elements
void update(GameState *state) {
  memcpy(state->screen, state->old_screen, sizeof(state->screen));
  handle_player(state);
  update_all_elements(state);
  ++state->count;
}

// Renders the screen based on the current state
void render(GameState *state) {
  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      char c = state->screen[j][i];
      if (c == '$' || c == 'S') {
        int l = ((state->count + 5 * j + 7 * i) % 16) / 8;
        printf("\e[%d;%dH", j + 1, i + 1);
        if (l == 0) {
          printf("\e[48;2;10;10;40m\e[38;2;153;51;255m$");
        } else {
          printf("\e[48;2;10;10;40m\e[38;12;33;61;255m$");
        }
        continue;
      }
      if (state->old_screen[j][i] != state->screen[j][i]) {
        printf("\e[%d;%dH", j + 1, i + 1);
        switch (c) {
        case '\n':
          printf("\n");
          break;
        case 'X':
          printf("\e[48;2;51;51;81m\e[38;2;91;91;91mX");
          break;
        case '.':
          printf("\e[48;2;80;76;60m\e[38;2;51;0;25m ");
          break;
        case ' ':
          printf("\e[48;2;10;10;40m ");
          break;
        case 'O':
          printf("\e[48;2;10;10;40m\e[38;2;202;198;194mO");
          break;
        case 'o':
          printf("\e[48;2;10;10;40m\e[38;2;202;198;194mo");
          break;
        case '@':
          printf("\e[48;2;10;10;40m\e[38;2;235;51;51m@");
          break;
        case 'E':
          printf("\e[48;2;10;10;40m\e[38;2;251;251;15mE");
        default:
          break;
        }
      }
    }
  }
  fflush(stdout);
}

// Get the player co-ordinates from the Current state
void find_player_position(GameState *state) {
  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      if (state->screen[j][i] == '@') {
        state->pos_x = i;
        state->pos_y = j;
        return;
      }
    }
  }
}

// Loads the level file into the game and reads in into the game state
void load_level(GameState *state, char* level) {
  FILE *f = fopen(level, "r");
  if (!f) {
    fprintf(stderr, "Failed to open level file");
    exit(EXIT_FAILURE);
  }
  if (fread(state->screen, 1, MAX_X * MAX_Y, f) != MAX_X * MAX_Y) {
    fprintf(stderr, "Failed to read level file");
    fclose(f);
    exit(EXIT_FAILURE);
  }
  render(state);
  find_player_position(state);
  memcpy(state->old_screen, state->screen, sizeof(state->screen));
  fclose(f);
}

// Prints the end message with result
void print_end_message(GameState *state) {
  printf("\e[%d;%dH", MAX_Y + 2, 1);
  if (state->dead) {
    printf("You died! Better luck next time!");
  }
  if (state->won) {
    printf("You won! You collected %d gems!", state->gems_collected);
  }
}

// Gets the level file name from the command line args
char* get_level(int argc, char* argv[]) {
  if (argc != 2) {
    return "";
  }

  return argv[1];
}

