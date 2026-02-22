#include "game.c"
#include "game.h"
#include <stdio.h>

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
  tb_set_output_mode(TB_OUTPUT_TRUECOLOR);

  GameState state = {0};
  load_level(&state, level);
  state.width = tb_width();
  state.height = tb_height();

  while (true) {
    struct tb_event ev;
    int ev_type = tb_peek_event(&ev, 100);
    if (ev_type == TB_ERR_NO_EVENT) {
      state.key = 0; // no input, just timeout.
    } else if (ev_type == TB_EVENT_RESIZE) {
      state.width = ev.w;
      state.height = ev.h;
    } else if (ev_type < 0) {
      break; // Error
    } else {
      // Handle input
      if (ev.type == TB_EVENT_KEY) {
        if (ev.key == TB_KEY_CTRL_Q) {
          break;
        }
      }
      handle_input(&state, &ev);
    }
    
    update(&state);
    render(&state);

    if (state.won || state.dead) {
      break;
    }

    memcpy(state.old_screen, state.screen, sizeof(state.screen));
  }

  tb_shutdown();

  print_end_message(&state);
}
