#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
 
int main(int argc, char** argv) {
    
    int p_rand = 0;

    // Устанавливаем генератор случайных чисел
    int srand_param = time(NULL);
    if (argc > 1) {
        srand_param = atoi(argv[1]);
    }
    srand(srand_param);

    // Генерация случайного числа от 1 до 10
    p_rand = 1 + rand()%(10 - 1 + 1);

    // Если сработала вероятность 0.1 - тогда вывести сообщение
    if (p_rand == 10) {
        printf("Сработало!!!\n");
    }
    //printf("%d\n", srand_param);
    
}