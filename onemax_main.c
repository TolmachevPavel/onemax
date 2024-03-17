#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/* ===== OneMax ===== */

int main(int argc, char** argv) {
/* Отбор, но чуть попозже */

    int MAX_GENERATIONS = 50;   // Максимальное количество поколений

    //int max = 10;
    int n_from, n_max;  /* Левая и правая границы селекции */
    int max_length = 100;    /* Размер хромосомы */
    int p_size = 200;    /* Размер популяции */
    int i = 0;
    int pi = 0;
    int s_left;
    int p_tournament = 3;
    int pop_after_tour[p_tournament];
    int p_rand;
    int cInd = 0;   /* Счётчик для цикла скрещивания */
    int gene = 0;
    int p_mut = 0;  /* Случайное число для сравнения с вероятностью мутации */
    int math_all_gene = 0;
    int math_max_val = 0;
    int cnt_generations = 0;
    int vals_mean_max[MAX_GENERATIONS][MAX_GENERATIONS][MAX_GENERATIONS]; /* Хранение среднего и максимального значения */
    
    //n_from = 1; /* минимальная граница слева для селекции */
    //n_max = max_length-2; /* максимальная граница справа для селекции */

    /* Структура для хранения одного экземпляра */
    struct Pop
    {
        int gene[max_length];   /* Ген, в данном случае 1 или 0 */
        int cost;               /* Стоимость данного экземпляра (сумма значений генов) */
    } Pop;

    /* Структура для хранения итоговых значений */
    struct Itog
    {
        int generations;    /* Номер поколения */
        float avg;   /* Среднее */
        int max;    /* Максимальное */
    } Itog;

    struct Pop pop[p_size];         /* Сгенерированный набор экземпляров размером из конфига */
    struct Pop new_individ[2];      /* Два экземпляра для скрещивания */
    struct Pop new_pop[p_size];
    struct Pop the2pop[p_size];
    struct Itog itog[MAX_GENERATIONS];

    n_from = 2; /* минимальная граница слева для селекции */
    n_max = max_length-1; /* максимальная граница справа для селекции */

    // Устанавливаем генератор случайных чисел
    int srand_param = time(NULL);   /* Возвращает время в секундах начиная с 1 января 1970 */
    if (argc > 1) { 
        srand_param = atoi(argv[1]);    /* Если не указан параметр командной строки - взять текущее время в секундах */
    }
    srand(srand_param); /* При одном и том же srand_param - будут выдавать одни и те же псевдослучайные значения */

    /* ========== ГЕНЕРАЦИЯ ПЕРВОГО ПОКОЛЕНИЯ ========== */

    for (pi = 0; pi < p_size; pi++) {   /* Счётчик индивидов */
        pop[pi].cost = 0;
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            pop[pi].gene[i] = (rand() % (1 - 0 + 1)) + 0;   /* Генерирую случайное 1 или 0 */
            pop[pi].cost = pop[pi].cost + pop[pi].gene[i];  /* Суммирую стоимость */
        }
    }

    /* Выводим всех сгенерированных индивидов на экран */
    //printf("Первая популяция:\n");
    for (pi = 0; pi < p_size; pi++) {
        //printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            //printf("%d", pop[pi].gene[i]);
        }
        //printf("; cost: %d", pop[pi].cost);
        //printf("\n");
    }
    //printf("\n");
    
    /* ========== ГЛАВНЫЙ ЦИКЛ ГЕНЕТИЧЕСКОГО АЛГОРИТМА ========== */

    for (cnt_generations = 0; cnt_generations < MAX_GENERATIONS; cnt_generations++) {

    /* ========== ТУРНИРНЫЙ ОТБОР ========== */

    /* В цикле размером макс.кол-во в поколении генерирую три случайных числа от 0 до макс.кол-во в поколении */
    for (pi = 0; pi < p_size; pi++) {
        //printf("%d-й индивид:\n", pi);
        int last_id, last_cost = -1;
        for (i = 0; i < p_tournament; i++) { // цикл из n раз
            // Генерация случайного числа от 0 до p_size - чтобы случайно выбрать одного из индивидов текущего поколения
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

    //printf("Поколение после турнирного отбора:\n");
    /* Выводим новых индивидов на экран */
    for (pi = 0; pi < p_size; pi++) {
        //printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            //printf("%d", new_pop[pi].gene[i]);
        }
        //printf("; cost: %d", new_pop[pi].cost);
        //printf("\n");
    }
    //printf("\n");

    /* ========== СКРЕЩИВАНИЕ ========== */

    //printf("Скрещивание с вероятностью 0.9:\n");
    for (pi = 0; pi < p_size; pi=pi+2) {
        //printf("pi: %d\n", pi);
        // Генерация случайного числа от 1 до 10
        p_rand = 1 + rand()%(10 - 1 + 1);
        // Если сработала вероятность 0.9 - произвести скрещивание
        //printf("p_rand: %d ", p_rand);
        

        /* Генерация случайного числа в промежутке ОТ и ДО */
        /* Случайным образом выбираю точку скрещивания, слева от которой гены останутся нетронутыми, справа - поменяются с другой парой */
        /* Пример: */
        /* 0110<точка_скрещивания>1001 -> 0110.1101 */
        /* 1110<точка_скрещивания>1101 -> 1110.1001 */
        s_left = n_from + rand()%(n_max - n_from);
        // printf("left: %d\n", s_left);
        // printf("right: %d", max_length - s_left - 1);
        // printf("\n");

        /* Одноточечное случайное скрещивание */

        /* Сначала дублируем всё что слева от точки скрещивания */
        //printf("(s_left-1): %d\n",s_left-1);
        for (cInd = 0; cInd < 2; cInd++) {
            if (p_rand != 10) {
                //printf("%d. Первая часть: ", pi+cInd);
                for(i=0; i < s_left; i++) {
                    the2pop[pi+cInd].gene[i] = new_pop[pi+cInd].gene[i];
                    //printf("%d",the2pop[pi+cInd].gene[i]);
                }
                //printf("\n");
                /* Обрабатываем вторые куски - то, что справа меняем местами между парами */
                /* Очередная пара - один и два*/
                //printf("%d. Вторая часть, первый: ", pi);
                for(i=s_left; i < max_length; i++) {
                    the2pop[pi].gene[i] = new_pop[pi+1].gene[i];
                    //printf("%d",the2pop[pi].gene[i]);
                }
                //printf("\n");
                //printf("%d. Вторая часть, второй: ", pi);
                for(i=s_left; i < max_length; i++) {                    
                    the2pop[pi+1].gene[i] = new_pop[pi].gene[i];
                    //printf("%d",the2pop[pi+1].gene[i]);
                }
                //printf("\n");
            }
            else {
                //printf("\nБез скрещивания:\n");
                //printf("%d. Первая часть: ", pi+cInd);
                for(i=0; i < max_length; i++) {
                    the2pop[pi+cInd].gene[i] = pop[pi+cInd].gene[i];
                    //printf("%d",the2pop[pi+cInd].gene[i]);
                }
                //printf("\n");
            }
        }
        //printf("\n");
                
    }
    //printf("\n");
    
    /* Выводим всех сгенерированных индивидов на экран */
    //printf("Популяция после скрещивания:\n");
    for (pi = 0; pi < p_size; pi++) {
        the2pop[pi].cost = 0;
        //printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            //printf("%d", the2pop[pi].gene[i]);
            the2pop[pi].cost = the2pop[pi].cost + the2pop[pi].gene[i];
        }
        //printf("; cost: %d", the2pop[pi].cost);
        //printf("\n");
    }
    //printf("\n");

    /* ========== МУТАЦИЯ ========== */

    for (pi = 0; pi < p_size; pi++) {
        // Генерация случайного числа от 1 до 100 %
        p_rand = 1 + rand()%(100 - 1 + 1);
        //printf("%d: Вероятность мутации: %d\n", pi, p_rand);
        // Если сработала вероятность 10% - произвести мутацию в случайном гене
        
        if (p_rand < 11) {
            // Генерация случайного числа от 0 до max_length-1
            p_mut = 0 + rand()%(max_length - 0);
            //printf("Ген: %d\n", p_mut);
            gene = the2pop[pi].gene[p_mut];
            if (gene == 0)
                the2pop[pi].gene[p_mut] = 1;
            else {
                the2pop[pi].gene[p_mut] = 0;
            }
        }
    }
    //printf("\n");
    /* Выводим всех сгенерированных индивидов на экран */
    math_all_gene = 0;
    math_max_val = -1;
    //printf("Популяция после мутации:\n");
    for (pi = 0; pi < p_size; pi++) {
        the2pop[pi].cost = 0;
        //printf("№ %d gene: ", pi);
        for(i = 0; i < max_length; ++i) {
            //printf("%d", the2pop[pi].gene[i]);
            the2pop[pi].cost = the2pop[pi].cost + the2pop[pi].gene[i];
            math_all_gene = math_all_gene + the2pop[pi].gene[i];
        }
        //printf("; cost: %d", the2pop[pi].cost);
        //printf("\n");
        /* Получение максимальной стоимости */
            if (the2pop[pi].cost > math_max_val){
                math_max_val = the2pop[pi].cost;
                itog[cnt_generations].max = math_max_val;
            }
    }
    /* Статистика: Среднее и Максимальное в поколении */
    //printf("\nsum_genes: %.1f\n", (float)math_all_gene/p_size);
    //printf("max_cost: %d\n", math_max_val);
    itog[cnt_generations].generations = cnt_generations;
    itog[cnt_generations].avg = (float)math_all_gene/p_size;
    itog[cnt_generations].max = math_max_val;
    //printf("generations: %d, sum_genes: %.1f, max_cost: %d\n", cnt_generations, itog[cnt_generations].avg, itog[cnt_generations].max);

    /* Копирование текущей популяции в структуру первой популяции и повторение цикла */
    //printf("\n");
    int r_ind = 0;
    for (pi = 0; pi < p_size; pi++) {   /* Счётчик индивидов */
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            pop[pi].gene[i] = the2pop[pi].gene[i];
            //printf("%d", pop[pi].gene[i]);
        }
    //printf("\n");        

    }
    //printf("\n");
    // // Вывод на экран
    // for (pi = 0; pi < p_size; pi++) {
    //     printf("gene: ");
    //     for(i = 0; i < max_length; ++i) {
    //         printf("%d", pop[pi].gene[i]);}
    //     //printf("; cost: %d", pop[pi].cost);
    //     printf("\n");
    // }
    }

    printf("\n");
    for (i=0; i < MAX_GENERATIONS; i++) {
       //printf("generations: %d, sum_genes: %.1f, max_cost: %d\n", i+1, itog[i].avg, itog[i].max);
       printf("%d| %.1f| %d\n", i+1, itog[i].avg, itog[i].max);
    }
}