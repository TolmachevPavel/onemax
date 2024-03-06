#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
 
int main(int argc, char** argv) {
/* Отбор, но чуть попозже */

    int max = 10;
    int n_from, n_max;
    int max_length = 10;    /* Размер хромосомы */
    int p_size = 10;    /* Размер популяции */
    int i = 0;
    int pi = 0;
    int s_left;
    int p_tournament = 3;
    int pop_after_tour[p_tournament];
    int p_rand;

    /* Структура для хранения одного экземпляра */
    struct Pop
    {
        int gene[max_length];   /* Ген, в данном случае 1 или 0 */
        int cost;               /* Стоимость данного экземпляра (сумма значений генов) */
    } Pop;

    struct Pop pop[p_size];         /* Сгенерированный набор экземпляров размером из конфига */
    struct Pop new_individ[2];      /* Два экземпляра для скрещивания */
    struct Pop new_pop[p_size];

    n_from = 2; /* минимальная граница слева для селекции */
    n_max = max_length-1; /* максимальная граница справа для селекции */

    // Устанавливаем генератор случайных чисел
    int srand_param = time(NULL);   /* Возвращает время в секундах начиная с 1 января 1970 */
    if (argc > 1) { 
        srand_param = atoi(argv[1]);    /* Если не указан параметр командной строки - взять текущее время в секундах */
    }
    srand(srand_param); /* При одном и том же srand_param - будут выдавать одни и те же псевдослучайные значения */

    // Заполняем массив значениями

    for (pi = 0; pi < p_size; pi++) {   /* Счётчик индивидов */
        pop[pi].cost = 0;
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            pop[pi].gene[i] = (rand() % (1 - 0 + 1)) + 0;   /* Генерирую случайное 1 или 0 */
            pop[pi].cost = pop[pi].cost + pop[pi].gene[i];  /* Суммирую стоимость */
        }
    }

    /* Выводим всех сгенерированных индивидов на экран */
    printf("Первая популяция:\n");
    for (pi = 0; pi < p_size; pi++) {
        printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            printf("%d", pop[pi].gene[i]);}
        printf("; cost: %d", pop[pi].cost);
        printf("\n");
    }
    printf("\n");
    
    /* Отбор */
    /* В цикле размером макс.кол-во в поколении генерирую три случайных числа от 1 до макс.кол-во в поколении */
    for (pi = 0; pi < p_size; pi++) {
        //printf("%d-й индивид:\n", pi);
        int last_id, last_cost = -1;
        for (i = 0; i < p_tournament; i++) { // цикл из n раз
            // Генерация случайного числа от 1 до 10
            // s_left = n_from + rand()%(n_max - n_from + 1);
            p_rand = 0 + rand()%((p_size-1) - 0 + 1);    // сгенерировать случайный индекс и по нему получить индивида из текущего пула
            //printf("p_rand: %d, cost: %d\n", p_rand, pop[p_rand].cost);
            // Выбираем лучшего из троих
            if (pop[p_rand].cost > last_cost) {
                //printf("last_id_old: %d; curr_id: %d",last_cost, pop[p_rand].cost);
                last_id = p_rand;
                last_cost = pop[p_rand].cost;
            }
        }
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            new_pop[pi].gene[i] = pop[last_id].gene[i];
            //printf("%d",pop[last_id].gene[i]);
        } 
    //printf("\nThe Best %d, cost: %d\n", last_id, last_cost);
    new_pop[pi].cost = last_cost;
    }

    printf("Поколение после турнирного отбора:\n");
    /* Выводим новых индивидов на экран */
    for (pi = 0; pi < p_size; pi++) {
        printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            printf("%d", new_pop[pi].gene[i]);}
        printf("; cost: %d", new_pop[pi].cost);
        printf("\n");
    }
    printf("\n");

    printf("Скрещивание с вероятностью 0.9:\n");
    for (pi = 0; pi < p_size/2; pi++) {
        // Генерация случайного числа от 1 до 10
        p_rand = 1 + rand()%(10 - 1 + 1);
        // Если сработала вероятность 0.1 - тогда вывести сообщение
        printf("p_rand: %d ", p_rand);
        if (p_rand != 10) {
            printf("Скрещивание!\n");
        }    
    }
    printf("\n");

}