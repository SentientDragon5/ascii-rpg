#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 4
#define MAX_ENEMY_STR_LEN 40
#define FLAVOR_TEXT_LEN 40
#define ENEMY_DELIMITER ";"

// Define the Vector2 struct
typedef struct {
    int x;
    int y;
    int type;
    char* flavor_text;
} Enemy;

typedef void(*functions_t)(void);

// Define constants for up, down, left, and right vectors

// Function prototypes
char* do_action(int player_x, int player_y, Enemy enemies[MAX_ENEMIES]);

void create_enemies(Enemy enemies[MAX_ENEMIES], char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN]);

void read_enemies_file(char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN], char *filename);

void enter_battle(void);
void read_text(void);
void teleport(void);

#endif
