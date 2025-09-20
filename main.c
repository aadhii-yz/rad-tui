#include "game.c"
#include <stdio.h>

#define TB_IMPL
#include "termbox2.h"

int main(int argc, char *argv[]) {
  char *level = get_level(argc, argv);
  if (strcmp(level, "") == 0) {
    printf("Level should be the first parameter.\n");
    return 1;
  }

  if (tb_init() != TB_OK) {
    fprintf(stderr, "Termbox initalization failed\n");
    return 1;
  }

  GameState state = {.pos_x = 5, .pos_y = 5};

  printf("\e[2J");
  load_level(&state, level);

  while (true) {
    struct tb_event ev;
    int ev_type = tb_peek_event(&ev, 100);
    if (ev_type == TB_ERR_NO_EVENT) {
      state.key = 0; // no input, just timeout.
    } else if (ev_type < 0) {
      break; // Error
    } else {
      // Handle input
      if (ev.type == TB_EVENT_KEY) {
        if (ev.key == TB_KEY_CTRL_Q) {
          break;
        }
      }
    }

    read_input(&state);
    update(&state);
    if (state.won || state.dead) {
      print_end_message(&state);
      break;
    }

    render(&state);

    memcpy(state.old_screen, state.screen, sizeof(state.screen));
  }

  tb_shutdown();
}
