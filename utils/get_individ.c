#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
int main(int argc, char** argv) {

/* Генерация одного индивида, состоящего из двоичных цифр */
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

// Устанавливаем генератор случайных чисел
    int srand_param = 1;
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

/* Вывод хромосом на экран */
for (pi = 0; pi < p_size; pi++) {
    printf("gene: ");
    for(i = 0; i < max_length; ++i) {
        printf("%d", pop[pi].gene[i]);}
    printf("; cost: %d", pop[pi].cost);
    printf("\n");
}

}
