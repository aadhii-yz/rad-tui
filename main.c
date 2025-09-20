#include <stdio.h>
#include <time.h>
#include "game.c"

#define TB_IMPL
#include "termbox2.h"

int main(int argc, char* argv[]) {
  char* level = get_level(argc, argv);
  if (strcmp(level, "") == 0) {
    printf("Level should be the first parameter.\n");
    return 1;
  }

  if (tb_init() != TB_OK) {
    fprintf(stderr, "Termbox initalization failed\n");
    return 1; 
  }

  struct timespec req = {};
  struct timespec rem = {};

  GameState state = {.pos_x = 5, .pos_y = 5};
  struct tb_event ev;

  printf("\e[2J");
  load_level(&state, level);

  clock_t start, end;

  while (!state.end) {
    tb_poll_event(&ev);
    if (ev.type == TB_EVENT_KEY) {
      if (ev.key == TB_KEY_CTRL_Q) {
        state.end = true;
        continue;
      }
    }
    start = clock();

    read_input(&state);
    update(&state);
    if (state.won || state.dead) {
      print_end_message(&state);
      break;
    }

    render(&state);

    memcpy(state.old_screen, state.screen, sizeof(state.screen));

    end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    if (time_taken > SPEED)
      continue;

    req.tv_sec = 0;
    req.tv_nsec = (SPEED - time_taken) * 1000000000;
    nanosleep(&req, &rem);
  }

  tb_shutdown();
}

