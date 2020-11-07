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
  int execucaoId; // ID da execução a qual esse parâmetro está vinculado (será utilizado para colocar o resultado das funções no buffer de resultados)
} Param;

typedef struct execucao { // Para cada execução agendada na função agendarExecucao, será criada uma instância desta Struct
  void *genericFunc;
  Param funcParams;
  int threadId;
} Execucao;

// Inicialização dos Mutexes e suas variáveis de condição.
pthread_mutex_t mutexBufferExecucao = PTHREAD_MUTEX_INITIALIZER; // Mutex relacionado ao buffer de execuções
pthread_mutex_t mutexBufferResult = PTHREAD_MUTEX_INITIALIZER; // Mutex relacionado ao buffer de resultados
pthread_cond_t condExecucao = PTHREAD_COND_INITIALIZER; // Variavél de condição para o buffer de execução
pthread_cond_t condResult = PTHREAD_COND_INITIALIZER; // Variavél de condição para o buffer de resultados

// Variáveis globais que serão utilizadas no decorrer da questão
Execucao *bufferExecucao;
int statusBufferExecucoes = 0; // Tamanho atual do buffer de execucoes
int *bufferResult;
int statusBufferResult = 0; // Tamanho atual do buffer de resultados

int qtdExecucoes = 0; // Irá funcionar como o ID das execuções, além disso, usaremos essa variável para comparar com o tamanho máximo do buffer e conferir se chegamos no limite
int statusThreads[N];

int agendarExecucao(void *funexec, Param funcParams) {  // Primeira função pedida pela questão, aqui colocamos uma nova execução (tarefa) no buffer de execuções e retornamos o ID dessa execução
  Execucao request;
  request.funcParams = funcParams;
  request.genericFunc = funexec;

  pthread_mutex_lock(&mutexBufferExecucao); // Travando o mutex para que outras threads não tenham acesso ao recurso compartilhado (buffer e variáveis globais) enquanto mexemos nele

  printf("\e[0;101m Agendando requisicao %d...\e[0m\n", qtdExecucoes);
  request.funcParams.execucaoId = qtdExecucoes;
  bufferExecucao[qtdExecucoes] = request;

  if(qtdExecucoes == BUFFER_SIZE) qtdExecucoes = 0; // O buffer funciona de forma parecida a um array circular, portanto, caso cheguemos ao seu final, devemos retornar ao início
  if(statusBufferExecucoes == 1) pthread_cond_broadcast(&condExecucao); // Caso essa seja a primeira execução dentro do buffer, devemos acordar as threads consumidoras
  qtdExecucoes++;
  statusBufferExecucoes++;
  if(qtdExecucoes == BUFFER_SIZE) qtdExecucoes = 0;
  if(statusBufferExecucoes == 1) pthread_cond_broadcast(&condExecucao); // Aqui conferimos novamente para atestar que as condições não foram atingidas após o incremento das variáveis

  pthread_mutex_unlock(&mutexBufferExecucao);
  return request.funcParams.execucaoId;
}

int pegarResultadoExecucao(int execucaoId) { // Segunda função pedida pela questão, aqui checamos se há algum resultado de execuções no buffer de resultados, se sim, retornamos esse resultado, se não, adormecemos as threads até termos um resultado
  int result;

  pthread_mutex_lock(&mutexBufferResult); // Travando o mutex para que outras threads não tenham acesso ao recurso compartilhado (buffer e variáveis globais) enquanto mexemos nele

  while(statusBufferResult == 0 || bufferResult[execucaoId] < 0) { // Enquanto o buffer estiver vazio ou então o resultado dessa execução em específico não estiver disponível, esperamos
    pthread_cond_wait(&condResult, &mutexBufferResult);
  }

  printf("\e[44m Pegando o resultado do id %d...\e[0m\n", execucaoId);
  result = bufferResult[execucaoId];
  bufferResult[execucaoId] = 0;
  statusBufferResult--;

  pthread_mutex_unlock(&mutexBufferResult);
  return result;
}

void *funexecAddMod10(Param params) {
  int result = (params.a + params.b + params.c) % 10;

  pthread_mutex_lock(&mutexBufferResult);

  bufferResult[params.execucaoId] = result;
  statusBufferResult++;

  pthread_cond_signal(&condResult); // Avisa às threads que um resultado foi adicionado ao buffer de resultados
  pthread_mutex_unlock(&mutexBufferResult);

  statusThreads[params.execucaoId] = -1;
  printf("funexecAddMod10 emitiu novo resultado, thread %d está livre.\n", params.execucaoId);
  pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
  int i;
  pthread_t threads[N];

  for(i = 0; i < N; i++) statusThreads[i] = -1; // Iniciando todas as threads com o status -1 para indicar que elas não estão ocupadas, portanto pode receber outra funexec

  pthread_exit(NULL);
  return 0;
}
