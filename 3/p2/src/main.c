#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int parse_and_check_file(char* file, char* symbols, int* gear_ratio_sum);
int get_number_in_string_from_position(char* str, int pos, int* num);
int remove_eof_from_string(char* str);

#define DEBUG
int main(void){
  int sum;
  parse_and_check_file("input.txt", "*", &sum);
  printf("SUM = %d\n", sum);
  return 0;
}

int parse_and_check_file(char* file, char* symbols, int* gear_ratio_sum){
  *gear_ratio_sum = 0;

  FILE* pparts = fopen(file, "r");
  if(pparts == NULL){
    printf("Didnt find %s\n", file);
    return -1;
  }
  const int BUFFER_SIZE = 300;
  char* buffer_prev = calloc(BUFFER_SIZE, sizeof(char));
  char* buffer = malloc(BUFFER_SIZE * sizeof(char));
  char* buffer_next = calloc(BUFFER_SIZE, sizeof(char));
  int has_reached_end = 0;

  if(fgets(buffer_next, BUFFER_SIZE, pparts) == NULL) has_reached_end = 1;
  remove_eof_from_string(buffer_next);

  while(has_reached_end != 1){
    strcpy(buffer, buffer_next); // atual <- prox

    if(fgets(buffer_next, BUFFER_SIZE, pparts) == NULL) has_reached_end = 1;
    remove_eof_from_string(buffer_next);
    
    #ifdef DEBUG
    //printf("Line: %s\n", buffer);
    #endif

    // Procurar por simbolos
    // Se encontrado, determinar quais os numeros em torno
    // somar esses numeros
    char * symbol;
    symbol = strpbrk(buffer, symbols);
    while (symbol != NULL){
      int pos = (int)(symbol - buffer);
      int temp;
      int prev_temp;
      #ifdef DEBUG
      printf("[%c] in pos %d\n", *symbol, pos);
      #endif
      //Para nao somar multiplas vezes o mesmo numero, o seguinte deve ser notado:
      //Ou N - M
      //Ou - N -
      //Geram numeros unicos, portanto
      //Se N M P, por N, M eh descontado, e por M, P eh descontado
      //Top 3
      int amount_of_numbers = 0;
      int gear_ratio = 1;
      if(get_number_in_string_from_position(buffer_prev, pos - 1, &temp) == 0){
	      gear_ratio *= temp;
	      amount_of_numbers++;
      }
      prev_temp = temp;
      if((get_number_in_string_from_position(buffer_prev, pos, &temp) == 0) && temp != prev_temp){
	      gear_ratio *= temp;
	      amount_of_numbers++;
      }
      prev_temp = temp;
      if((get_number_in_string_from_position(buffer_prev, pos + 1, &temp) == 0) && temp != prev_temp){
	      gear_ratio *= temp;
	      amount_of_numbers++;
      }
      //Middle 2
      if(get_number_in_string_from_position(buffer, pos - 1, &temp) == 0){
	      gear_ratio *= temp;
	      amount_of_numbers++;
      }
      if(get_number_in_string_from_position(buffer, pos + 1, &temp) == 0){
	      gear_ratio *= temp;
	      amount_of_numbers++;
      }
      //Bottom 3
      if(has_reached_end == 0){
        if(get_number_in_string_from_position(buffer_next, pos - 1, &temp) == 0){
	      gear_ratio *= temp;
	      amount_of_numbers++;
        }
        prev_temp = temp;
        if((get_number_in_string_from_position(buffer_next, pos, &temp) == 0) && temp != prev_temp){
	      gear_ratio *= temp;
	      amount_of_numbers++;
        }
        prev_temp = temp;
        if((get_number_in_string_from_position(buffer_next, pos + 1, &temp) == 0) && temp != prev_temp){
	      gear_ratio *= temp;
	      amount_of_numbers++;
        }
      }
      if(amount_of_numbers == 2) *gear_ratio_sum += gear_ratio;

      symbol = strpbrk(symbol+1, symbols);
    }
    strcpy(buffer_prev, buffer); // prev <- atual 
  }
  return 0;
}

int get_number_in_string_from_position(char* str, int pos, int* num){
  *num = -1;
  if(str == NULL) return -1;
  if(*(str + pos) > '9' || *(str + pos) < '0') return -1;
  //Procurar inicio do numero
  char* start = str + pos;
  while(start > str && *start <= '9' && *start >= '0') start--;
  if(*start > '9' || *start < '0') start++; //Achou inicio do numero
  *num = (int)strtol(start, NULL, 10);
  #ifdef DEBUG
  printf("Found number %d in pos %d (%s)\n", *num, pos, start);
  #endif
  return 0;
}
int remove_eof_from_string(char* str){
  if(str == NULL) return -1;
  char* eol_loc = strchr(str,'\n');
  if(eol_loc != NULL) *eol_loc = '\0'; // Substitiu o \n do fim da string lida pelo char de termino
  eol_loc = strchr(str,'\r');
  if(eol_loc != NULL) *eol_loc = '\0'; // Substitiu o \r do fim da string lida pelo char de termino
  return 0;
}

