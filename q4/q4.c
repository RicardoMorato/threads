#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
  Nesta questão serão necessários dois buffers e, consequentemente, dois Mutexes.
  Serão necessários dois buffers pois, em um precisamos ter uma fila de execuções que ainda serão realizadas e
  em outro precisamos ter uma lista de todos os resultados das execuções que já foram concluídas.

  Tendo estes dois buffers, precisamos agora de dois Mutexes, um para cada buffer,
  a fim de evitar que duas ou mais threads tenham acesso a uma região crítica (os buffers).
*/

#define N 5 // Número de processadores/núcleos do sistema, portanto, também representa a quantidade de threads do programa
#define BUFFER_SIZE 10 // Tamanho máximo do Buffer

typedef struct param { // Struct pedida pela questão que irá servir como parâmetro de uma função em uma execução
  int a, b, c;
} Param;

typedef struct Execucao { // Para cada execução agendada na função agendarExecucao, será criada uma instância desta Struct
  void *func;
  Param funcParams;
  int execucaoId;
  int threadId;
};

// Inicialização dos Mutexes e suas variáveis de condição.
pthread_mutex_t mutexBufferExecucao = PTHREAD_MUTEX_INITIALIZER; // Mutex relacionado ao buffer de execuções
pthread_mutex_t mutexBufferResult = PTHREAD_MUTEX_INITIALIZER; // Mutex relacionado ao buffer de resultados
pthread_cond_t condExecucao = PTHREAD_COND_INITIALIZER; // Variavél de condição para o buffer de execução
pthread_cond_t condResult = PTHREAD_COND_INITIALIZER; // Variavél de condição para o buffer de resultados

int agendarExecucao(void *funexec, Param funcParams) {  // Primeira função pedida pela questão, aqui colocamos uma nova execução (tarefa) no buffer de execuções e retornamos o ID dessa execução
}


int main(int argc, char *argv[]) {
  pthread_t threads[N];

  pthread_exit(NULL);
  return 0;
}
