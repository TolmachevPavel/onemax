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
    int p_size = 10;
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
    for (pi = 0; pi < p_size; pi++) {
        printf("gene: ");
        for(i = 0; i < max_length; ++i) {
            printf("%d", pop[pi].gene[i]);}
        printf("; cost: %d", pop[pi].cost);
        printf("\n");
    }
    printf("\n");
    
    /* Отбор */
    for (pi = 0; pi < p_size; pi++) {
        for (i = 0; i < p_tournament; i++) { // цикл из n раз
            // Генерация случайного числа от 1 до 10
            p_rand = 1 + rand()%(p_size - 1 + 1);    // сгенерировать случайный индекс и по нему получить индивида из текущего пула
            pop_after_tour[i] = pop[pi].gene;
            printf("%d\n", pop_after_tour[i]);
        // оценить полученные индивиды
        // выбрать лучшего индивида -> записать его в новый пул
        
        // На выходе - p_size элементов в обновлённом пуле
        }
    }

}