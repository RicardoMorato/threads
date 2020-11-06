#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5 // Número de processadores/núcleos do sistema, portanto, também representa a quantidade de threads do programa
#define BUFFER_SIZE 10 // Tamanho máximo do Buffer

typedef struct param { // Struct pedida pela questão que irá servir como parâmetro de uma função em uma execução
  int a, b, c;
} Param;

typedef struct Execucao { // Para cada execucao agendada na funcao agendarExecucao, será criada uma instância desta Struct
  void *func;
  Param funcParams;
  int execucaoId;
  int threadId;
};

int main(int argc, char *argv[]) {return 0;}
