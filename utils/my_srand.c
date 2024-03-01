#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
int main(int argc, char** argv) {

/* Демонстрация srand, генерация двоичных чисел */

  int srand_param = 1;
  
  if (argc > 1) {
    srand_param = atoi(argv[1]);
  }

  srand(srand_param);
  printf("%d\n", (rand() % (1 - 0 + 1)) + 0);
  printf("%d\n", (rand() % (1 - 0 + 1)) + 0);
  printf("%d\n", (rand() % (1 - 0 + 1)) + 0);
  printf("%d\n", (rand() % (1 - 0 + 1)) + 0);
  printf("%d\n", (rand() % (1 - 0 + 1)) + 0);
  
  printf("srand_param = %d\n", srand_param);
}
