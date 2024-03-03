#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
 
int main(int argc, char** argv) {
/* Отбор, но чуть попозже */

    int max = 10;
    int n_from, n_max;
    int max_length = 10;
    int p_size = 5;
    int population[p_size][max_length];
    int i = 0;
    int pi = 0;

    struct Pop
    {
        int gene[max_length];
        int cost;
    } Pop;

    struct Pop pop[p_size];

    n_from = 2;
    n_max = 12;

    // Устанавливаем генератор случайных чисел
    int srand_param = time(NULL);
    if (argc > 1) {
        srand_param = atoi(argv[1]);
    }
    srand(srand_param);

    // Заполняем массив значениями

    for (pi = 0; pi < p_size; pi++) {
        pop[pi].cost = 0;
        for(i=0; i < max_length; i++) {
            pop[pi].gene[i] = (rand() % (1 - 0 + 1)) + 0;
            pop[pi].cost = pop[pi].cost + pop[pi].gene[i];
        }
    }

    /* Выбираем случайный кусок из индивида */
    for (pi = 0; pi < p_size; pi++) {
        printf("gene: ");
        for(i = 0; i < max_length; ++i) {
            printf("%d", pop[pi].gene[i]);}
        printf("; cost: %d", pop[pi].cost);
        printf("\n");
    }

    for (i=0; i < max; i++)
    {
        /* Генерация случайного числа в промежутке ОТ и ДО */
        //printf("%d\n", n_from + rand()%(n_max - n_from + 1));
    }
    printf("\n");
}