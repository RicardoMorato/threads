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

void inicia_tabela() {
    int i, j;
    for(i=0;i<L; i++) {
        for(j=0; j<TAM_L; j++) {
            tabela[i][j] = ' ';
        }
        tabela[i][j] = '\0';
    }
    system("clear");
    for(int i=0; i<L; i++) {
        if(i == L-2) {   
            printf("%s%s%s\033[m\n", cores[i], branco, tabela[i]);
        }
        else {
            printf("%s%s%s\033[m\n", cores[i], preto, tabela[i]);
        }
    }
    printf("\033[7A");
    system("sleep 2");
}

int main() {
    inicia_tabela();
    return 0;
}


