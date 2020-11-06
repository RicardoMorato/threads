#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5
#define BUFFER_SIZE 10

long int counter = 0;

typedef struct elem{
  // void *(*funexec)(void *);
  int funexec;
  struct elem *next;
} Elem;

typedef struct {
  unsigned int bufferSize, bufferStatus;
  Elem *head, *last;
} Buffer;

typedef struct {
  void* _arg;
} FunexecParams;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

Buffer *newBuffer(unsigned int bufferSize) {
  Buffer *tmp = (Buffer *) malloc(sizeof(Buffer));
  Elem *sentinel = (Elem *) malloc(sizeof(Elem));

  sentinel->funexec = -1;
  sentinel->next = NULL;

  tmp->head = tmp->last = sentinel;
  tmp->bufferSize = bufferSize;
  tmp->bufferStatus = 0;

  return tmp;
}

void printBuffer(Buffer *Q) {
  Elem *cur = Q->head;
  while(cur->next != NULL) {
    printf("%d ", cur->next->funexec);
    cur = cur->next;
  }
  printf("\n");
  printf("Valor Head: %d, Valor Last: %d\n", Q->head->funexec, Q->last->funexec);
}

void bufferPush(Buffer *Q, int funexec) {
  Q->bufferStatus++;
  Elem *tail = Q->last;
  Elem *new = (Elem *) malloc(sizeof(Elem));

  new->funexec = funexec;
  new->next = NULL;
  tail->next = new;
  Q->last = new;
}

int bufferPop(Buffer *Q) {
  if(Q->head->next == NULL) {
    printf("BUFFER VAZIO");
    return -1;
  }

  int elem_deleted = 0;
  Q->bufferStatus--;
  Elem *aux = Q->head->next;
  elem_deleted = aux->funexec;
  Q->head->next = aux->next;

  free(aux);
  if(Q->head->next == NULL) {
    Q->last = Q->head;
  }

  printf("Deleted %d\n", elem_deleted);

  return elem_deleted;
}

void agendarExecucao(void *(*funexec)(void *), FunexecParams params) { // Essa função adiciona funexec ao buffer de rotinas.

}

int main(int argc, char *argv[]) {
  Buffer *Q = newBuffer(BUFFER_SIZE);
  bufferPush(Q, 10);
  bufferPush(Q, 100);
  bufferPush(Q, 1000);
  bufferPush(Q, 10000);
  int a = bufferPop(Q);
  printBuffer(Q);

  return 0;
}
