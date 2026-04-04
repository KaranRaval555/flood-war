#include "base.h"
#include <raylib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 800
#define COLORS 10
#define MAX_MOVES 25
#define CELLS 16
#define CELL_SIZE (WIDTH / CELLS)

Color board[CELLS][CELLS];
int moves = 0;

typedef enum {
  PLAYING,
  WIN,
  LOSS
} GameState;

GameState state = PLAYING;

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

int playerRegion[CELLS][CELLS];

void print_board() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      printf("%d", playerRegion[i][j]); // simplified
    }
    printf("\n");
  }
  printf("------\n");
}

void reset_region() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      playerRegion[i][j] = 0;
    }
  }
}

void init_board() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      int n = GetRandomValue(0, 9);
      board[i][j] = colors[n];
    }
  }
  reset_region();
  playerRegion[0][0] = 1;
}

void display_board() {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      DrawRectangle(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE, board[i][j]);
    }
  }
}

void prefilled_region(Color new_color) {
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(playerRegion[i][j]){
        board[i][j] = new_color;
      } 
    }
  }
}

void dfs(Color color, int row, int col) {
  if (row < 0 || row >= CELLS || col < 0 || col >= CELLS  // out of boundary
      || playerRegion[row][col]                  // already visited
      || !ColorIsEqual(board[row][col], color)   // cell doesn't have current color in area
     ) return;

  playerRegion[row][col] = 1;

  dfs(color, row - 1, col);
  dfs(color, row + 1, col);
  dfs(color, row, col - 1);
  dfs(color, row, col + 1);
}

void fill_region(Color color) {
  prefilled_region(color);
  reset_region();
  dfs(color, 0, 0);
}

int count_cells() {
  int count = 0;
  for (int i = 0; i < CELLS; i++) {
    for (int j = 0; j < CELLS; j++) {
      if(playerRegion[i][j]) count++;
    }
  }
  return count;
}

void player_move() {
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mousePos = GetMousePosition();
    int col = (int)mousePos.x / CELL_SIZE;
    int row = (int)mousePos.y / CELL_SIZE;
    board[0][0] = board[row][col];
    fill_region(board[row][col]);
    moves++;
  }
}


void GameLoop() {
  const char* winTxt = "VICTORY!";
  const char* lossTxt = "GAME OVER!";
  const char* resetTxt = "Press [R] to Play Again";

  int fontSize = 80;
  moves = 0;

  int textWidth1 = MeasureText(winTxt, fontSize);
  int textWidth2 = MeasureText(lossTxt, fontSize);
  int textWidth3 = MeasureText(resetTxt, 25);

  init_board();

  while(!WindowShouldClose()) {
    if(state == PLAYING) {
      player_move();
      if(count_cells() >= (CELLS * CELLS) / 2){
        state = WIN;
      }
      else if (moves > MAX_MOVES) { 
        state = LOSS;
      }
    }
    else {
      if(IsKeyPressed(KEY_R)) {
        moves = 0;
        init_board();
        state = PLAYING;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    switch (state) {
      case PLAYING:
        display_board();
        break;
      case WIN:
        ClearBackground(BLACK);
        DrawText(winTxt, WIDTH/2 - textWidth1/2, HEIGHT/2 - fontSize/2, fontSize, GREEN);
        DrawText(resetTxt, WIDTH/2 - textWidth3/2, HEIGHT/2 + 50, 25, RAYWHITE);
        break;
      case LOSS:
        ClearBackground(BLACK);
        DrawText(lossTxt, WIDTH/2 - textWidth2/2, HEIGHT/2 - fontSize/2, fontSize, RED);
        DrawText(resetTxt, WIDTH/2 - textWidth3/2, HEIGHT/2 + 50, 25, RAYWHITE);
        break;
      default:
        break;
    }
    EndDrawing();
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, "Jork");
  SetTargetFPS(45);
  GameLoop();
  CloseWindow();
}
