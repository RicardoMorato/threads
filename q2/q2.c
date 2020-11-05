#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define L 7
#define TAM_L 26
#define N 8 
#define T 4

char *arquivos[8] = {"teste1.txt", 
                     "teste2.txt", 
                     "teste3.txt", 
                     "teste4.txt", 
                     "teste5.txt", 
                     "teste6.txt", 
                     "teste7.txt", 
                     "teste8.txt"};
char tabela[L][TAM_L+1];
char *cores[L] = {"\033[48;2;255;0;0",
                  "\033[48;2;255;255;0",
                  "\033[48;2;0;0;255",
                  "\033[48;2;255;0;255",
                  "\033[48;2;0;255;0",
                  "\033[48;2;0;0;0",
                  "\033[48;2;0;186;186"};
char* preto = ";30m"; char* branco = "m";

pthread_mutex_t mutex_linhas[L], mutex_tabela;
int ids[T];

int main() {
    return 0;
}


