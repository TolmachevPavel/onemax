## Здача OneMax

### О чем

OneMax - это задача с битовой строкой, цель которой — максимизировать количество единиц в двоичной строке.

### Как я решал задачу

Решаю с помощью генетического алгоритма используя турнирный отбор, одноточечное скрещивание и мутацию.

### Пример итогового графика с решением зачачи OneMax
![](https://github.com/TolmachevPavel/onemax/blob/main/for_readme.jpeg)

### Как запускать программу

Есть один ключ командной строки:
- srand

srand и число после него служит для установки значения генератора случайных чисел. При указания одинакового числа - результаты работы программы будут одинаковыми. После srand число обязательно, если его не поставить - программа упадет при запуске.
debug - включает режим журналиварования всех операций работы программы, пока вывод идет на экран.

Пример:
```bash
./onemax_main srand 4
```

```./onemax_main srand 4               
Количество поколений: 100
Размер популяции: 100
Размер хромосомы: 100
Индивидов в турнирном отборе: 48
Вероятность скрещивания: 90
Вероятность мутации: 35


1; 59.4; 66
2; 64.5; 68
3; 67.3; 71
4; 70.2; 73
5; 72.1; 76
6; 74.6; 79
7; 77.6; 81
8; 80.0; 84
9; 82.1; 85
10; 84.2; 86
11; 85.3; 87
12; 86.7; 89
13; 88.1; 90
14; 89.0; 91
15; 90.3; 93
16; 92.2; 95
17; 94.0; 96
18; 95.2; 97
19; 96.4; 98
20; 97.4; 99
21; 98.5; 100

debug_mode = 0
srand_param = 4
```
В файл GA_logfile пишется лог проделанных операций:
```
Количество поколений: 3
Размер популяции: 10
Размер хромосомы: 10
Индивидов в турнирном отборе: 3
Вероятность скрещивания: 90
Вероятность мутации: 35

Первая популяция:
№ 0 gene: 1101001101; cost: 6
№ 1 gene: 1000010000; cost: 2
№ 2 gene: 1011101010; cost: 6
№ 3 gene: 1001101011; cost: 6
№ 4 gene: 1110101001; cost: 6
№ 5 gene: 0111001010; cost: 5
№ 6 gene: 0000110000; cost: 2
№ 7 gene: 1111101001; cost: 7
№ 8 gene: 1111011010; cost: 7
№ 9 gene: 0001010110; cost: 4


Поколение: 1. Турнирный отбор (из 3 штук):

0-й обновленный индивид:

0-й шаг турнирного отбора:
p_rand: 1, cost: 2
last_id_old: -1; curr_id: 2 
1-й шаг турнирного отбора:
p_rand: 1, cost: 2
...
debug_mode = 1
srand_param = 1710785666
```
### График
Автоматически перезаписывается файл GA_data, на основе которого тоже автоматически строится график в файле GA_onemax.jpeg. Для этого использую gnuplot и файл-определение графика ga_graph.gpi.
### Ограничения
Полно.
