// Includes :
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define TB_OPT_ATTR_W 32
#define TB_IMPL
#include "termbox2.h"

#include "game.h"

// Handle keyboard inputs
void handle_input(GameState *state, struct tb_event *ev) {
  state->key = 0;

  if (ev->type == TB_EVENT_KEY) {
    switch (ev->key) {
      case TB_KEY_ARROW_UP:
        state->key = 1;
        break;
      case TB_KEY_ARROW_DOWN:
        state->key = 2;
        break;
      case TB_KEY_ARROW_RIGHT:
        state->key = 3;
        break;
      case TB_KEY_ARROW_LEFT:
        state->key = 4;
        break;
    }
  }
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
  tb_set_clear_attrs(TB_DEFAULT, 0x0a0a28);  
  tb_clear();
  int dx = (state->width - MAX_X) / 2;
  int dy = (state->height - MAX_Y) / 2;
  
  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      char c = state->screen[j][i];
      uint32_t ch = c;
      uint32_t fg = 0;
      uint32_t bg = 0x0a0a28;
      
      switch (c) {
      case 'X':
        ch = ' ';
        bg = 0x333351;
        break;
      case '.':
        ch = ' ';
        bg = 0x504c3c;
        break;
      case ' ':
        break;
      case 'o':
      case 'O':
        fg = 0xcac6c2;
        break;
      case '@':
        fg = 0xeb3333;
        break;
      case 'E':
        fg = 0xfbfb0f;
        break;
      case '$':
      case 'S':
        ch = '$';
        fg = 0x213dff;
        break;
      default:
        continue;
      }
      tb_set_cell(dx + i, dy + j, ch, fg, bg);
    }
  }
  tb_present();
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
  printf("\e[2J\e[0;0H");
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

