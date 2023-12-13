#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int parse_and_check_file(char* file, int max_red, int max_blue, int max_green, int* possible_games_sum);
int parse_game_id_tok(char* game_id_tok, int* game_id);
int parse_game_data_tok(char* game_data_tok, int max_red, int max_green, int max_blue, int* is_game_possible);
int parse_game_data_subsets_tok(char* game_data_subsets_tok, int* red, int* blue, int* green);
int parse_game_data_subsets_cube_tok(char* game_data_subsets_cube_tok, int* amount, int* color);

#define DEBUG
int main(void){
  int sum;
  parse_and_check_file("input.txt", 12, 13, 14, &sum);
  printf("SUM = %d\n", sum);
  return 0;
}

int parse_and_check_file(char* file, int max_red, int max_green, int max_blue, int* possible_games_sum){
  *possible_games_sum = 0;

  FILE* pgames = fopen(file, "r");
  if(pgames == NULL){
    printf("Didnt find %s\n", file);
    return -1;
  }
  const int BUFFER_SIZE = 300;
  char* buffer = malloc(BUFFER_SIZE * sizeof(char));
  while(fgets(buffer, BUFFER_SIZE, pgames) != NULL){
    char* eol_loc = strchr(buffer,'\n');
    if(eol_loc != NULL) *eol_loc = '\0'; // Substitiu o \n do fim da string lida pelo char de termino
    eol_loc = strchr(buffer,'\r');
    if(eol_loc != NULL) *eol_loc = '\0'; // Substitiu o \r do fim da string lida pelo char de termino
    
    #ifdef DEBUG
    printf("Line: %s\n", buffer);
    #endif

    int game_id = -1;
    int is_game_possible = 1;

    char * game_and_rest_tok;
    char * buffer_rest = buffer;
    int current_part = 0;
    while((game_and_rest_tok = strtok_r(buffer_rest,":",&buffer_rest)) && current_part < 2){
      #ifdef DEBUG
      //printf("(parse_and_check_file) Tok = [%s]\n", game_and_rest_tok);
      #endif
      if(current_part == 0){ //Game ID
        parse_game_id_tok(game_and_rest_tok, &game_id);
        #ifdef DEBUG
        printf("Game Id = %d\n", game_id);
        #endif
      }
      else if(current_part == 1){
        parse_game_data_tok(game_and_rest_tok, max_red, max_green, max_blue, &is_game_possible);
        #ifdef DEBUG
        printf("\tIs game possible? %d\n", is_game_possible);
        #endif
      }
      current_part++; 
    }

    if(is_game_possible) *possible_games_sum += game_id;
  }
  return 0;
}

int parse_game_id_tok(char* game_id_tok, int* game_id){
  char * game_id_tok_rest = game_id_tok;
  char * tok;
  int current_tok = 0;
  while((tok = strtok_r(game_id_tok_rest," ",&game_id_tok_rest)) && current_tok < 2){
    #ifdef DEBUG
    //printf("(parse_game_id_tok) Tok = [%s]\n", tok);
    #endif
    //if(current_tok == 0) //"Game"
    if(current_tok == 1){//Game ID
      *game_id = (int)strtol(tok, NULL, 10);
    }
    current_tok++;
  }
  return 0;
}

int parse_game_data_tok(char* game_data_tok, int max_red, int max_green, int max_blue, int* is_game_possible){
  char * game_data_tok_rest = game_data_tok;
  char * tok;
  *is_game_possible = 1;
  while((tok = strtok_r(game_data_tok_rest,";",&game_data_tok_rest))){
    #ifdef DEBUG
    //printf("(parse_game_data_tok) Tok = [%s]\n", tok);
    #endif
    int red;
    int green;
    int blue;
    parse_game_data_subsets_tok(tok, &red, &green, &blue);
    if(red > max_red || green > max_green || blue > max_blue ) *is_game_possible = 0;
  }
  return 0;
  
}

int parse_game_data_subsets_tok(char* game_data_subsets_tok, int* red, int* green, int* blue){
  char * game_data_subsets_tok_rest = game_data_subsets_tok;
  char * tok;
  *red = 0;
  *green = 0;
  *blue = 0;
  while((tok = strtok_r(game_data_subsets_tok_rest,",",&game_data_subsets_tok_rest))){
    #ifdef DEBUG
    //printf("(parse_game_data_subsets_tok) Tok = [%s]\n", tok);
    #endif
    int amount;
    int color;
    parse_game_data_subsets_cube_tok(tok, &amount, &color);
    if(color == 0) *red+= amount;
    else if(color == 1) *green+= amount;
    else if(color == 2) *blue+= amount;
  }
  #ifdef DEBUG
  printf("\t\tRED: %d, GREEN %d, BLUE %d\n", *red, *green, *blue);
  #endif
  return 0;
}

int parse_game_data_subsets_cube_tok(char* game_data_subsets_cube_tok, int* amount, int* color){
  char * game_data_subsets_cube_tok_rest = game_data_subsets_cube_tok;
  char * tok;
  int current_tok = 0;
  while((tok = strtok_r(game_data_subsets_cube_tok_rest," ",&game_data_subsets_cube_tok_rest)) && current_tok < 2){
    #ifdef DEBUG
    //printf("(parse_game_data_subsets_cube_tok) Tok = [%s]\n", tok);
    #endif
    if(current_tok == 0){//Amount
      *amount = (int)strtol(tok, NULL, 10);
    }
    if(current_tok == 1){//Color
      if(strcmp(tok, "red") == 0) *color = 0;
      else if(strcmp(tok, "green") == 0) *color = 1;
      else if(strcmp(tok, "blue") == 0) *color = 2;
    }
    current_tok++;
  }
  return 0;
}
