#include <raylib.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 800
#define COLORS 10
#define CELLS 16
#define CELL_SIZE (WIDTH / CELLS)

Color board[CELLS][CELLS];
int playerRegion[CELLS][CELLS];

typedef struct {
  int id;
  int origin;
} Player;

typedef enum {
  PLAYING,
  END,
} GameState;

GameState state = PLAYING;

Player p1 = {
  .id = -1,
  .origin = 0
};

Player p2 = {
  .id = 1,
  .origin = CELLS - 1
};

Player *player;

Color colors[COLORS] = {
  {  248, 226, 185, 255}, // cream
  {  0, 255, 255, 255}, // cyan
  {  0, 128, 255, 255}, // electric blue
  {128,   0, 255, 255}, // violet
  {255,  64,   0, 255}, // neon orange
  {255, 255,   0, 255}, // electric yellow
  {  0, 255, 128, 255}, // neon green
  {255,   0, 255, 255}, // hot pink
  { 190, 33, 55, 255 }, // MAROON
  { 80,  80, 120, 255}  // muted accent
};

void reset_region(int val) {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(playerRegion[i][j] == val) playerRegion[i][j] = 0;
    }
  }
}

void init_board() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      int n = GetRandomValue(0, 9);
      board[i][j] = colors[n];
      board[i][j].a = 255;
      playerRegion[i][j] = 0;
    }
  }
  playerRegion[0][0] = -1;
  playerRegion[CELLS - 1][CELLS - 1] = 1;
}

void display_board() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      DrawRectangle(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE, board[i][j]);
    }
  }
}

void prefilled_region(int val, Color new_color) {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(playerRegion[i][j] == val){
        board[i][j] = new_color;
      } 
    }
  }
}

bool is_visited(int val) {
  return val != 0;
}

bool InEnemyRegion(int row, int col) {
  return playerRegion[row][col] == -player->id;
}

bool InFilledRegion(int row, int col) {
  return InEnemyRegion(row, col) || playerRegion[row][col] == player->id;
}

void dfs(Color color, int row, int col) {
  if (row < 0 || row >= CELLS || col < 0 || col >= CELLS  // out of boundary
      || is_visited(playerRegion[row][col])                  // already visited
      || !ColorIsEqual(board[row][col], color)   // cell doesn't have current color in area
      || InEnemyRegion(row, col)
     ) return;

  playerRegion[row][col] = player->id;

  dfs(color, row - 1, col);
  dfs(color, row + 1, col);
  dfs(color, row, col - 1);
  dfs(color, row, col + 1);
}

void fill_region(Color color) {
  prefilled_region(player->id, color);
  reset_region(player->id);
  dfs(color, player->origin, player->origin);
}

int count_cells() {
  int count = 0;
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(playerRegion[i][j] == player->id) count++;
    }
  }
  return count;
}

void switch_player() {
  player = player->id == -1 ? &p2 : &p1 ;
}

void blur() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(InEnemyRegion(i, j)) { 
         board[i][j].a = 220;
      }
      else {
         board[i][j].a = 255;
      }
    }
  }
}

bool player_move() {
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mousePos = GetMousePosition();
    int col = (int)mousePos.x / CELL_SIZE;
    int row = (int)mousePos.y / CELL_SIZE;
    if(InFilledRegion(row, col)) return false;
    board[player->origin][player->origin] = board[row][col];
    fill_region(board[row][col]);
    blur();
    return true;
  }
  return false;
}


void GameLoop() {
  const char* winTxt;
  int fontSize = 80;

  player = &p1;
  init_board();

  while(!WindowShouldClose()) {
    if(state == PLAYING) {
      if (player_move()) {
        if(count_cells() >= (((CELLS * CELLS) / 2) - 16)) {
          state = END;
        } else {
          switch_player();
        }
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if(state == PLAYING) {
      display_board();
    }
    else {
      ClearBackground(BLACK);
      winTxt = TextFormat("Player %d WINS!", player->id == -1 ? 1 : 2);
      int textWidth = MeasureText(winTxt, fontSize);

      DrawText(winTxt, WIDTH/2 - textWidth/2, HEIGHT/2 - fontSize/2, fontSize, GREEN);
    }
    EndDrawing();
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, "Flood War");
  SetTargetFPS(45);
  GameLoop();
  CloseWindow();
}
