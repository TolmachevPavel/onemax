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
    int p_size = 2;
    int population[p_size][max_length];
    int i = 0;
    int pi = 0;
    int s_left;

    struct Pop
    {
        int gene[max_length];
        int cost;
    } Pop;

    struct Pop pop[p_size];
    struct Pop new_individ[2];

    n_from = 2; // минимальная граница слева для селекции
    n_max = max_length-1; // максимальная граница справа для селекции

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

    /* Выводим всех сгенерированных индивидов */
    for (pi = 0; pi < p_size; pi++) {
        printf("gene: ");
        for(i = 0; i < max_length; ++i) {
            printf("%d", pop[pi].gene[i]);}
        printf("; cost: %d", pop[pi].cost);
        printf("\n");
    }
    printf("\n");
    /* Генерация случайного числа в промежутке ОТ и ДО */
    s_left = n_from + rand()%(n_max - n_from + 1);
    printf("left: %d\n", s_left);
    printf("right: %d", max_length - s_left);
    printf("\n");

    /* Одноточечное скрещивание */

    /* Сначала дублируем всё что слева */
    //printf("(s_left-1): %d\n",s_left-1);
    for (pi = 0; pi < 2; pi++) {
        for(i=0; i < s_left-1; i++) {
            new_individ[pi].gene[i] = pop[pi].gene[i];
        }
    }

    for(i=s_left-1; i < max_length; i++) {
        new_individ[0].gene[i] = pop[1].gene[i];
    }
    for(i=s_left-1; i < max_length; i++) {
        new_individ[1].gene[i] = pop[0].gene[i];
    }

    /* Выводим всех сгенерированных индивидов */
    for (pi = 0; pi < p_size; pi++) {
        printf("gene: ");
        for(i = 0; i < max_length; ++i) {
            printf("%d", new_individ[pi].gene[i]);}
        //printf("; cost: %d", new_individ[pi].cost);
        printf("\n");
    }

}