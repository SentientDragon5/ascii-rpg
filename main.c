#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "enemy.h"

#define MAP_WIDTH 30
#define MAP_HEIGHT 15
#define VIEW_WIDTH 11
#define VIEW_HEIGHT 11

void set_terminal_mode();
void print_map(char map[MAP_HEIGHT][MAP_WIDTH], int player_x, int player_y, Enemy enemies[MAX_ENEMIES]);
void read_map_file(char map[MAP_HEIGHT][MAP_WIDTH], char *filename);
void update_player_position(char input, char map[MAP_HEIGHT][MAP_WIDTH], int *player_x, int *player_y);
void print_color(char c);
void clear_screen();
int get_collision(char c);
void render(char map[MAP_HEIGHT][MAP_WIDTH], int player_x, int player_y, Enemy enemies[MAX_ENEMIES]);

void print_line();
void print_lives(int lives);
int get_len(char *str);
void print_flavor_text(int player_x, int player_y, Enemy enemies[MAX_ENEMIES]);

// enemy.h
extern char* do_action(int player_x, int player_y, Enemy enemies[MAX_ENEMIES]);
extern void create_enemies(Enemy enemies[MAX_ENEMIES], char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN]);
extern void read_enemies_file(char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN], char *filename);

int main(void) {
  set_terminal_mode();
  char map[MAP_HEIGHT][MAP_WIDTH];
  int player_x = 10, player_y = 10;
  //Enemies
  char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN];
  Enemy enemies[MAX_ENEMIES];

  // Read map
  read_map_file(map, "map.txt");
  // Read Enemies
  read_enemies_file(enemies_text, "enemies.txt");
  create_enemies(enemies, enemies_text);

  // -1 render
  render(map, player_x, player_y, enemies);

  char input;
  do {
    input = getchar();
    update_player_position(input, map, &player_x, &player_y);
    render(map, player_x, player_y, enemies);

  } while (input != 'e');

  return 0;
}

void render(char map[MAP_HEIGHT][MAP_WIDTH], int player_x, int player_y, Enemy enemies[MAX_ENEMIES]){
  clear_screen();

  print_lives(3);
  print_line();
  print_map(map, player_x, player_y, enemies); 
  print_line();
  print_flavor_text(player_x, player_y, enemies);
}

void print_map(char map[MAP_HEIGHT][MAP_WIDTH], int player_x, int player_y, Enemy enemies[MAX_ENEMIES]) {
  
  int view_x = player_x - VIEW_WIDTH / 2;
  int view_y = player_y - VIEW_HEIGHT / 2;
  
  //Constrain the viewer to fit the bounds
  if(view_x < 0)
    view_x = 0;
  if(view_x + VIEW_WIDTH > MAP_WIDTH)
    view_x = VIEW_WIDTH - MAP_WIDTH;
  if(view_y < 0)
    view_y = 0;
  if(view_y + VIEW_HEIGHT > MAP_HEIGHT)
    view_y = MAP_HEIGHT - VIEW_HEIGHT;
  
  for (int i = view_y; i < view_y + VIEW_HEIGHT; i++) {
    for (int j = view_x; j < view_x + VIEW_WIDTH; j++) {
      int printed = 0;
      // If player at position, print
      if (j == player_x && i == player_y) {
        printf("\033[0;35m");//36
        printf("%2c ", '@');
        printf("\033[0m");
        continue;
      } 
      // If enemy at position, print
      for(int k=0; k < MAX_ENEMIES; k++){
        if(j == enemies[k].x && i == enemies[k].y){
          switch(enemies[k].type){
            case 1:
              printf("\033[0;34m");
              printf("%2c ", 'P');
              break;
            case 2:
              printf("\033[0;34m");
              printf("%2c ", 'O');
              break;
            default:
              printf("\033[0;31m");
              printf("%2c ", 'E');
              break;
          }
          printf("\033[0m");
          printed = 1;
          break;
        }
      }
      if(printed){
        continue;
      }
      // If tile at position, print, else print ' '
      if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
        print_color(map[i][j]);
      } else {
        printf("%2c ", ' ');
      }
    }
    printf("\n");
  }
}

void read_map_file(char map[MAP_HEIGHT][MAP_WIDTH], char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file '%s'. Exiting.\n", filename);
    exit(1);
  }

  for (int i = 0; i < MAP_HEIGHT; i++) {
    int j;
    for (j = 0; j < MAP_WIDTH; j++) {
      int c = fgetc(file);
      if (c == EOF) {
        printf("Unexpected end of file. Exiting.%d\n", c);
        exit(1);
      }
      map[i][j] = (char)c;
    }
    int newline = fgetc(file);
    if (newline != '\n' && newline != EOF) {
      printf("Unexpected character '%c' in file. Exiting.%d % d\n", newline, i,
             j);
      exit(1);
    }
  }

  fclose(file);
}

void update_player_position(char input, char map[MAP_HEIGHT][MAP_WIDTH], int *player_x, int *player_y) {
  switch (input) {
  case 'w':
    if(!get_collision(map[*player_y-1][*player_x])){
      break;
    }
    (*player_y)--;
    break;
  case 'a':
    if(!get_collision(map[*player_y][*player_x-1])){
      break;
    }
    (*player_x)--;
    break;
  case 's':
    if(!get_collision(map[*player_y+1][*player_x])){
      break;
    }
    (*player_y)++;
    break;
  case 'd':
    if(!get_collision(map[*player_y][*player_x+1])){
      break;
    }
    (*player_x)++;
    break;
  }
}

void clear_screen() {
  printf("\033[2J");           // ANSI escape code to clear screen
  printf("\033[%d;%dH", 0, 0); // move cursor to top-left corner
}

int get_collision(char c){
  int walkable_len = 6;
  char walkable[] = "._+*+$";
  for(int i=0; i<walkable_len; i++){
    if(c == walkable[i])
    {
      return 1;
    }
  }
  return 0;
}

void print_color(char c) {
  switch (c) {
  case '.':
    printf("\033[0;32m"); // Green for grass
    break;
  case '~':
    printf("\033[0;34m"); // Blue for water
    break;
  case '#':
    printf("\033[0;37m"); // Gray for walls
    break;
  case '_':
    printf("\033[0;33m"); // Yellow for sand
    break;
  case '^':
    printf("\033[1;37m"); // Bright gray for mountains
    break;
  case '*':
    printf("\033[0;35m"); // Magenta for flowers
    break;
  case '+':
    printf("\033[0;31m"); // Red for health packs
    break;
  case '$':
    printf("\033[1;33m"); // Gold for treasure chests
    break;
  default:
    printf("\033[0m"); // Reset color
    break;
  }

  printf("%2c ", c);
  printf("\033[0m"); // Reset color
}
void set_terminal_mode() {
  struct termios tattr;
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void print_line(){ 
  for(int i=0; i<VIEW_WIDTH; i++){
    printf("---");
  }
  printf("\n");
}

void print_lives(int lives){
  printf("\033[0;31m");
  for(int i=0; i<lives; i++){
    printf(" <3");
  }
  printf("\033[0;36m");
  for(int i=lives; i<VIEW_WIDTH - 2; i++){
    printf("   ");
  }
  printf("LVL %d", 1);
  printf("\n");
  printf("\033[0m"); // Reset color
}

void print_flavor_text(int player_x, int player_y, Enemy enemies[MAX_ENEMIES]){
  char* flavor_text = do_action(player_x, player_y, enemies);
  int len = get_len(flavor_text);
  printf("%s",flavor_text);
  for(int i=len; i<(VIEW_WIDTH*3 - 8); i++){
    printf(" ");
  }
  printf("(%d, %d)\n", player_x, player_y);
}

int get_len(char *str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}
