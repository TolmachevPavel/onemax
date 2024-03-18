#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* ===== OneMax ===== */

int main(int argc, char** argv) {
/* Отбор, но чуть попозже */

    int MAX_GENERATIONS = 100;   // Количество поколений
    int P_MUTATION = 35;
    int max_length = 100;    /* Размер хромосомы */
    int p_size = 100;    /* Размер популяции */
    int p_tournament = 48;  /* Индивидов в турнирном отборе */
    int P_CROSSOVER = 90;   /* Вероятность скрещивания */

    int n_from, n_max;  /* Левая и правая границы селекции */
    int i = 0;
    int pi = 0;
    int s_left;
    int pop_after_tour[p_tournament];
    int p_rand;
    int cInd = 0;   /* Счётчик для цикла скрещивания */
    int gene = 0;
    int p_mut = 0;  /* Случайное число для сравнения с вероятностью мутации */
    int math_all_gene = 0;
    int math_max_val = 0;
    int cnt_generations = 0;
    int vals_mean_max[MAX_GENERATIONS][MAX_GENERATIONS][MAX_GENERATIONS]; /* Хранение среднего и максимального значения */
    int currMax = 0;
    FILE *file;
    
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

    // Проверка параметров командной строки

    int debug_mode = 0; // Режим отладки. По умолчанию отключён
    int srand_param = time(NULL); // Пар-р использующийся для установки генератора случайных чисел, возвращает время в секундах начиная с 1 января 1970

    /* Проверяю параметры командной строки, по результату либо устанавливаю либо нет debug_mode и srand_param*/
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "debug") == 0) {
            debug_mode = 1;
        }
        if (strcmp(argv[i], "srand") == 0) {
            srand_param = atoi(argv[i+1]);
        }
    }
    // Окончание проверки параметров

    // Устанавливаем генератор случайных чисел
    srand(srand_param); /* При одном и том же srand_param - будут выдавать одни и те же псевдослучайные значения */

    printf("Количество поколений: %d\nРазмер популяции: %d\nРазмер хромосомы: %d\nИндивидов в турнирном отборе: %d\nВероятность скрещивания: %d\nВероятность мутации: %d\n\n",MAX_GENERATIONS,p_size,max_length,p_tournament,P_CROSSOVER,P_MUTATION);

    /* ========== ГЕНЕРАЦИЯ ПЕРВОГО ПОКОЛЕНИЯ ========== */

    for (pi = 0; pi < p_size; pi++) {   /* Счётчик индивидов */
        pop[pi].cost = 0;
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            pop[pi].gene[i] = (rand() % (1 - 0 + 1)) + 0;   /* Генерирую случайное 1 или 0 */
            pop[pi].cost = pop[pi].cost + pop[pi].gene[i];  /* Суммирую стоимость */
        }
    }

    /* Выводим всех сгенерированных индивидов на экран */
    if (debug_mode == 1) {
        printf("Первая популяция:\n");
    }
    for (pi = 0; pi < p_size; pi++) {
        if (debug_mode == 1) {
            printf("№ %d gene: ", pi);
        }
        for(i = 0; i < max_length; ++i) {
            if (debug_mode == 1) {
                printf("%d", pop[pi].gene[i]);
            }
        }
        
        if (debug_mode == 1) {
            printf("; cost: %d", pop[pi].cost);
            printf("\n");
        }
    }
    if (debug_mode == 1) {
        printf("\n");
    }
  
    /* ========== ГЛАВНЫЙ ЦИКЛ ГЕНЕТИЧЕСКОГО АЛГОРИТМА ========== */
    //cnt_generations = 0;
    //for (cnt_generations = 0; cnt_generations < MAX_GENERATIONS; cnt_generations++) {
    do {
        cnt_generations++;
        currMax = 0;

    /* ========== ТУРНИРНЫЙ ОТБОР ========== */

    /* В цикле размером макс.кол-во в поколении генерирую три случайных числа от 0 до макс.кол-во в поколении */
    if (debug_mode == 1) {
        printf("\nПоколение: %d. Турнирный отбор (из %d штук):\n", cnt_generations, p_tournament);
    }
    for (pi = 0; pi < p_size; pi++) {
        if (debug_mode == 1) {
            printf("\n%d-й обновленный индивид:\n", pi);
        }
        int last_id, last_cost = -1;
        for (i = 0; i < p_tournament; i++) { // цикл из n раз
            if (debug_mode == 1) {
                printf("\n%d-й шаг турнирного отбора:\n", i);
            }
            // Генерация случайного числа от 0 до p_size - чтобы случайно выбрать одного из индивидов текущего поколения
            p_rand = 0 + rand()%((p_size-1) - 0 + 1);    // сгенерировать случайный индекс и по нему получить индивида из текущего пула
            if (debug_mode == 1) {
                printf("p_rand: %d, cost: %d\n", p_rand, pop[p_rand].cost);
            }
            // Выбираем лучшего из троих
            if (pop[p_rand].cost > last_cost) {
                if (debug_mode == 1) {
                    printf("last_id_old: %d; curr_id: %d ",last_cost, pop[p_rand].cost);
                }
                last_id = p_rand;
                last_cost = pop[p_rand].cost;
            }
        }
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            new_pop[pi].gene[i] = pop[last_id].gene[i];
            if (debug_mode == 1) {
                printf("%d",pop[last_id].gene[i]);
            }
        } 
    if (debug_mode == 1) {
        printf("\nThe Best %d, cost: %d\n", last_id, last_cost);
    }
    new_pop[pi].cost = last_cost;
    }

    if (debug_mode == 1) {
        printf("\nПоколение после турнирного отбора:\n");
    }
    /* Выводим новых индивидов на экран */
    for (pi = 0; pi < p_size; pi++) {
        if (debug_mode == 1) {
            printf("№ %d gene: ", pi);
        }
        for(i = 0; i < max_length; ++i) {
            if (debug_mode == 1) {
                printf("%d", new_pop[pi].gene[i]);
            }
        }
        if (debug_mode == 1) {
            printf("; cost: %d", new_pop[pi].cost);
            printf("\n");
        }
    }
    if (debug_mode == 1) {
        printf("\n");
    }

    /* ========== СКРЕЩИВАНИЕ ========== */

    if (debug_mode == 1) {
        printf("Скрещивание с вероятностью 0.9:\n\n");
    }
    for (pi = 0; pi < p_size; pi=pi+2) {
        if (debug_mode == 1) {
            printf("Пара номер: pi: %d\n", pi);
        }
        // Генерация случайного числа от 1 до 10
        p_rand = 1 + rand()%(100 - 1 + 1);
        // Если сработала вероятность 0.9 - произвести скрещивание
        if (debug_mode == 1) {
            printf("p_rand: %d ", p_rand);
        }        

        /* Генерация случайного числа в промежутке ОТ и ДО */
        /* Случайным образом выбираю точку скрещивания, слева от которой гены останутся нетронутыми, справа - поменяются с другой парой */
        /* Пример: */
        /* 0110<точка_скрещивания>1001 -> 0110.1101 */
        /* 1110<точка_скрещивания>1101 -> 1110.1001 */
        s_left = n_from + rand()%(n_max - n_from);
        if (debug_mode == 1) {
            printf("left: %d\n", s_left);
            printf("right: %d", max_length - s_left - 1);
            printf("\n");
        }

        /* Одноточечное случайное скрещивание */

        /* Сначала дублируем всё что слева от точки скрещивания */
        if (debug_mode == 1) {
            printf("(s_left-1): %d\n",s_left-1);
        }
        for (cInd = 0; cInd < 2; cInd++) {
            if (p_rand <= P_CROSSOVER) {
                if (debug_mode == 1) {
                    printf("Сработала вероятность 0.9 - производится одноточечное скрещивание\n");
                    printf("%d. Первая часть: ", pi+cInd);
                }
                for(i=0; i < s_left; i++) {
                    the2pop[pi+cInd].gene[i] = new_pop[pi+cInd].gene[i];
                    if (debug_mode == 1) {
                        printf("%d",the2pop[pi+cInd].gene[i]);
                    }
                }
                if (debug_mode == 1) {
                    printf("\n");
                }
                /* Обрабатываем вторые куски - то, что справа меняем местами между парами */
                /* Очередная пара - один и два*/
                if (debug_mode == 1) {
                    printf("%d. Вторая часть, первый: ", pi);
                }
                for(i=s_left; i < max_length; i++) {
                    the2pop[pi].gene[i] = new_pop[pi+1].gene[i];
                    if (debug_mode == 1) {
                        printf("%d",the2pop[pi].gene[i]);
                    }
                }
                if (debug_mode == 1) {
                    printf("\n");
                    printf("%d. Вторая часть, второй: ", pi);
                }
                for(i=s_left; i < max_length; i++) {                    
                    the2pop[pi+1].gene[i] = new_pop[pi].gene[i];
                    if (debug_mode == 1) {
                        printf("%d",the2pop[pi+1].gene[i]);
                    }
                }
                if (debug_mode == 1) {
                    printf("\n");
                }
            }
            else {
                if (debug_mode == 1) {
                    printf("\nБез скрещивания:\n");
                    printf("%d. Первая часть: ", pi+cInd);
                }
                for(i=0; i < max_length; i++) {
                    the2pop[pi+cInd].gene[i] = pop[pi+cInd].gene[i];
                    if (debug_mode == 1) {
                        printf("%d",the2pop[pi+cInd].gene[i]);
                    }
                }
                if (debug_mode == 1) {
                    printf("\n");
                }
            }
        }
        if (debug_mode == 1) {
            printf("\n");
        }
                
    }
    if (debug_mode == 1) {
        printf("\n");
    }
    
    /* Выводим всех сгенерированных индивидов на экран */
    if (debug_mode == 1) {
        printf("Популяция после скрещивания:\n");
    }
    for (pi = 0; pi < p_size; pi++) {
        the2pop[pi].cost = 0;
        if (debug_mode == 1) {
            printf("№ %d gene: ", pi);
        }
        for(i = 0; i < max_length; ++i) {
            if (debug_mode == 1) {
                printf("%d", the2pop[pi].gene[i]);
            }
            the2pop[pi].cost = the2pop[pi].cost + the2pop[pi].gene[i];
        }
        if (debug_mode == 1) {
            printf("; cost: %d", the2pop[pi].cost);
            printf("\n");
        }
    }
    if (debug_mode == 1) {
        printf("\n");
    }

    /* ========== МУТАЦИЯ ========== */

    for (pi = 0; pi < p_size; pi++) {
        // Генерация случайного числа от 1 до 100 %
        p_rand = 1 + rand()%(100 - 1 + 1);
        if (debug_mode == 1) {
            printf("%d: Вероятность мутации: %d\n", pi, p_rand);
        }
        // Если сработала вероятность 10% - произвести мутацию в случайном гене
        
        if (p_rand <= P_MUTATION) {
            // Генерация случайного числа от 0 до max_length-1
            p_mut = 0 + rand()%(max_length - 0);
            if (debug_mode == 1) {
                printf("Ген: %d\n", p_mut);
            }
            gene = the2pop[pi].gene[p_mut];
            if (gene == 0)
                the2pop[pi].gene[p_mut] = 1;
            else {
                the2pop[pi].gene[p_mut] = 0;
            }
        }
    }
    if (debug_mode == 1) {
        printf("\n");
    }
    
    /* Выводим всех сгенерированных индивидов на экран */
    math_all_gene = 0;
    math_max_val = -1;
    if (debug_mode == 1) {
        printf("Популяция после мутации:\n");
    }
    for (pi = 0; pi < p_size; pi++) {
        the2pop[pi].cost = 0;
        if (debug_mode == 1) {
            printf("№ %d gene: ", pi);
        }
        for(i = 0; i < max_length; ++i) {
            if (debug_mode == 1) {
                printf("%d", the2pop[pi].gene[i]);
            }
            the2pop[pi].cost = the2pop[pi].cost + the2pop[pi].gene[i];
            math_all_gene = math_all_gene + the2pop[pi].gene[i];
        }
        if (debug_mode == 1) {
            printf("; cost: %d", the2pop[pi].cost);
            printf("\n");
        }
        /* Получение максимальной стоимости */
            if (the2pop[pi].cost > math_max_val){
                math_max_val = the2pop[pi].cost;
                itog[cnt_generations].max = math_max_val;
            }
    }
    /* Статистика: Среднее и Максимальное в поколении */
    itog[cnt_generations].generations = cnt_generations;
    itog[cnt_generations].avg = (float)math_all_gene/p_size;
    itog[cnt_generations].max = math_max_val;
    currMax = math_max_val;
    
    if (debug_mode == 1) {
        printf("\nsum_genes: %.1f\n", (float)math_all_gene/p_size);
        printf("max_cost: %d\n", math_max_val);
        printf("generations: %d, sum_genes: %.1f, max_cost: %d\n", cnt_generations, itog[cnt_generations].avg, itog[cnt_generations].max);        
    }

    /* Копирование текущей популяции в структуру первой популяции и повторение цикла */
    if (debug_mode == 1) {
        printf("\nКопирование текущей популяции в структуру первой популяции и повторение цикла\n");
    }
    int r_ind = 0;
    for (pi = 0; pi < p_size; pi++) {   /* Счётчик индивидов */
        for(i=0; i < max_length; i++) { /* Счётчик генов у конкретного индивида */
            pop[pi].gene[i] = the2pop[pi].gene[i];
            //printf("%d", pop[pi].gene[i]);
        }
        pop[pi].cost = the2pop[pi].cost;
    if (debug_mode == 1) {
        //printf("\n");        
    }

    }
    if (debug_mode == 1) {
        printf("\n");
        // Вывод на экран
        for (pi = 0; pi < p_size; pi++) {
            printf("gene: ");
            for(i = 0; i < max_length; ++i) {
                printf("%d", pop[pi].gene[i]);}
                printf("; cost: %d", pop[pi].cost);
               printf("\n");
            }
        }
        //printf("cnt_generations: %d; currMax: %d\n",cnt_generations,currMax);
    } while (cnt_generations < MAX_GENERATIONS && currMax < max_length);

    file = fopen("GA_data", "w");
    printf("\n");
    i = 1;
    currMax = 0;
    for (i=1; i < MAX_GENERATIONS; i++) {
        if (debug_mode == 1) {
            printf("generations: %d, sum_genes: %.1f, max_cost: %d\n", i, itog[i].avg, itog[i].max);
        }
        printf("%d; %.1f; %d\n", i, itog[i].avg, itog[i].max);
        fprintf(file,"%d %.1f %d\n", i, itog[i].avg, itog[i].max);
        if (itog[i].max == max_length) {
            break;
        }
    }
    fclose(file);

    //if (debug_mode == 1) {
        printf("\ndebug_mode = %d\n", debug_mode);
        printf("srand_param = %d\n", srand_param);
    //}

    pid_t pid=fork();
    if (pid==0) {
        static char *argv[]={"gnuplot","ga_graph.gpi",NULL};
        execv("/bin/gnuplot",argv);
        exit(127);
    }
        
}