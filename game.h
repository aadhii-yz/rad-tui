#ifndef _game_h_
#define _game_h_

// Defines :
#ifdef RUN_TESTS

#define MAX_X 5
#define MAX_Y 4

#else

#define MAX_X 60
#define MAX_Y 26

#endif

// Game State :
typedef struct {
  int key;
  int pos_x;
  int pos_y;
  unsigned int count;
  int gems_collected;
  int dead;
  int won;
  char old_screen[MAX_Y][MAX_X];
  char screen[MAX_Y][MAX_X];
} GameState;

void handle_input(GameState *state, struct tb_event *ev);
void handle_player(GameState *state);
void handle_rocks_gems(GameState *state, int x, int y);
void handle_falling_rocks_gems(GameState *state, int x, int y);
void update_all_elements(GameState *state);
void update(GameState *state);
void render(GameState *state);
void find_player_position(GameState *state);
void load_level(GameState *state, char* level);
void print_end_message(GameState *state);
char* get_level(int argc, char* argv[]);

#endif // !_game_h_

