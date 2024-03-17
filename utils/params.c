#include <stdio.h>
 #include <string.h>
 int main(int argc, char **argv) {
     if(argc <= 1) {
         printf("syntax : \n\t%s --help or\n\t./p --add [a] [b]\n", argv[0]);
         return 0;
     }
     if(!strcmp(argv[1], "--help")) {
         printf("You requested help message.\n");
     } else if(!strcmp(argv[1], "--add")) {
         if(argc <= 3) {
             printf("'--add' operation requires two parameters.\n");
         } else {
             int a, b;
             if(sscanf(argv[2], "%d", &a) != 1 || sscanf(argv[3], "%d", &b) != 1) {
                 printf("'--add' operation requires two integer parameters.\n");
             } else {
                 printf("%d + %d = %d\n", a, b, a+b);
             }
         }
     } else {
         printf("Unknown parameter: '%s'. Type %s --help for help.\n", argv[1], argv[0]);
     }
     return 0;
 }
