#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
int main(int argc, char** argv) {

/* Демонстрация srand */

  int srand_param = 1;
  
  if (argc > 1) {
    srand_param = atoi(argv[1]);
  }

  srand(srand_param);
  printf("%d\n", rand());
  printf("%d\n", rand());
  printf("%d\n", rand());
  printf("%d\n", rand());
  printf("%d\n", rand());

  printf("%d\n", srand_param);
}
