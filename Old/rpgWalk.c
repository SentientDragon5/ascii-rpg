#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define MAP_WIDTH 29
#define MAP_HEIGHT 10
#define VIEW_WIDTH 11
#define VIEW_HEIGHT 11

void print_map(char map[MAP_HEIGHT][MAP_WIDTH], int view_y, int view_x);
void read_map_file(char map[MAP_HEIGHT][MAP_WIDTH], char *filename);
void update_view_position(char input, int *view_y, int *view_x);
void clear_screen();
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

int main() {
  set_terminal_mode();
  char map[MAP_HEIGHT][MAP_WIDTH];
  int view_y = 0, view_x = 0;

  read_map_file(map, "map.txt");
  clear_screen();
  print_map(map, view_y, view_x);

  char input;
  do {
    input = getchar();
    update_view_position(input, &view_y, &view_x);
    clear_screen();
    print_map(map, view_y, view_x);
  } while (input != 'e');

  return 0;
}

void print_map(char map[MAP_HEIGHT][MAP_WIDTH], int view_y, int view_x) {
  for (int i = view_y; i < view_y + VIEW_HEIGHT; i++) {
    for (int j = view_x; j < view_x + VIEW_WIDTH; j++) {
      if (j - view_x == VIEW_WIDTH / 2 && i - view_y == VIEW_HEIGHT / 2) {
        printf("\033[0;35m");//36
        printf("%2c ", '@');
        printf("\033[0m");
      } else if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
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

void update_view_position(char input, int *view_y, int *view_x) {
  switch (input) {
  case 'w':
    (*view_y)--;
    break;
  case 'a':
    (*view_x)--;
    break;
  case 's':
    (*view_y)++;
    break;
  case 'd':
    (*view_x)++;
    break;
  }
}

void clear_screen() {
  printf("\033[2J");           // ANSI escape code to clear screen
  printf("\033[%d;%dH", 0, 0); // move cursor to top-left corner
}

