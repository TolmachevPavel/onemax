#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) { 
 
    int debug_mode = 0; // Режим отладки. По умолчанию отключён
    int rand_param = 0; // Пар-р использующийся для установки генератора случайных чисел

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "debug") == 0) {
            debug_mode = 1;
        }
        if (strcmp(argv[i], "rand") == 0) {
            
            rand_param = atoi(argv[i+1]);
        }
    }

    printf("\ndebug_mode = %d\n", debug_mode);
    printf("rand_param = %d\n", rand_param);
}
