#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

functions_t FUNCTIONS[] = {enter_battle, read_text, teleport};

char* do_action(int player_x, int player_y, Enemy enemies[MAX_ENEMIES]) {
  char* flavor_text = malloc(FLAVOR_TEXT_LEN * sizeof(char));
  strncpy(flavor_text, " ", FLAVOR_TEXT_LEN - 1);
  
  for(int k=0; k < MAX_ENEMIES; k++){
    if(player_x == enemies[k].x && player_y == enemies[k].y){
      // Set flavor text
      strncpy(flavor_text, enemies[k].flavor_text, FLAVOR_TEXT_LEN);
      // Do action
      
      break;
    }
  }
  
  flavor_text[FLAVOR_TEXT_LEN - 1] = '\0';
  return flavor_text;
}

void create_enemies(Enemy enemies[MAX_ENEMIES], char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN]){
  
  for(int i=0; i<MAX_ENEMIES; i++){
    char *token;
    int arr[4];
    int j = 0;

    // Get the first token
    token = strtok(enemies_text[i], ENEMY_DELIMITER);

    // Walk through other tokens and store them in the integer array
    while (j < 3) {
        arr[j] = atoi(token);
        j++;
        token = strtok(NULL, ENEMY_DELIMITER);
    }

    enemies[i].x = arr[0];
    enemies[i].y = arr[1];
    enemies[i].type = arr[2];
    enemies[i].flavor_text = token;
  }
}

void read_enemies_file(char enemies_text[MAX_ENEMIES][MAX_ENEMY_STR_LEN], char *filename){
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file '%s'. Exiting.\n", filename);
    exit(1);
  }

  for (int i = 0; i < MAX_ENEMIES; i++) {
    int j = 0;
    int c;
    while((c = fgetc(file)) != '\n'){
      if (c == EOF) {
        printf("Unexpected end of file. Exiting.%d\n", c);
        break;
        //exit(1);
      }
      enemies_text[i][j] = (char)c;
      j++;
    }
  }

  fclose(file);
}

void enter_battle(void){}
void read_text(void){}
void teleport(void){}
