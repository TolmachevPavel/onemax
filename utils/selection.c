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
    int i = 0;
    int pi = 0;
    int s_left;

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
    
    /* Генерация случайного числа в промежутке ОТ и ДО */
    /* Случайным образом выбираю точку скрещивания, слева от которой гены останутся нетронутыми, справа - поменяются с другой парой */
    /* Пример: */
    /* 0110<точка_скрещивания>1001 -> 0110.1101 */
    /* 1110<точка_скрещивания>1101 -> 1110.1001 */
    s_left = n_from + rand()%(n_max - n_from + 1);
    printf("left: %d\n", s_left);
    printf("right: %d", max_length - s_left);
    printf("\n");

    /* Одноточечное случайное скрещивание */

    /* Сначала дублируем всё что слева от точки скрещивания */
    //printf("(s_left-1): %d\n",s_left-1);
    for (pi = 0; pi < 2; pi++) {
        for(i=0; i < s_left; i++) {
            new_individ[pi].gene[i] = pop[pi].gene[i];
            //printf("%d. Первая часть: <%d> ", pi, pop[pi].gene[i]);
        }
    //printf("\n");
    }
    /* Обрабатываем вторые куски - то, что справа меняем местами между парами */
    for(i=s_left; i < max_length; i++) {
        new_individ[0].gene[i] = pop[1].gene[i];
        //printf("1. Вторая часть: <%d> ", pop[1].gene[i]);
    }
    //printf("\n");
    for(i=s_left; i < max_length; i++) {
        new_individ[1].gene[i] = pop[0].gene[i];
        //printf("2. Вторая часть: <%d> ", pop[0].gene[i]);
    }
    printf("\n");

    /* Выводим обновлённой пары индивидов */
    for (pi = 0; pi < p_size; pi++) {
        printf("gene: ");
        for(i = 0; i < max_length; ++i) {
            printf("%d", new_individ[pi].gene[i]);}
        //printf("; cost: %d", new_individ[pi].cost);
        printf("\n");
    }

}