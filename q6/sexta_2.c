#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define QTD_CONSUMER 3
#define QTD_PRODUCER 3

const int tam_da_fila=10;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

typedef struct elem{
   int value;
   struct elem *prox;
}Elem;
 
typedef struct blockingQueue{
   unsigned int sizeBuffer, statusBuffer;
   Elem *head,*last;
}BlockingQueue;

void teste(BlockingQueue* fila){
    printf("elem_head: %d, elem_last: %d tam_total: %d,  tamanho_atual: %d\n", fila->head->value, fila->last->value, fila->sizeBuffer, fila->statusBuffer);
}

void printa_fila(BlockingQueue *fila){
    Elem* cur= fila->head;
    while(cur->prox!= NULL){
        printf("%d ", cur->prox->value);
        cur= cur->prox;
    }
    printf("\n");
    printf("valor_head: %d, valor_last: %d\n", fila->head->prox->value, fila->last->value);
}
void fila_push(BlockingQueue *fila, int valor){
    fila->statusBuffer++;
    Elem* cauda= fila->last;
    Elem* novo = (Elem *)  malloc(sizeof(Elem));
    novo->value= valor;    novo->prox= NULL;
    cauda->prox= novo;
    fila->last= novo;
}
int fila_pop(BlockingQueue *fila){
    if(fila->head->prox==NULL){
        printf("fila vazia!!\n");
        return -1;
    }
    int elem_deletado=0;
    fila->statusBuffer--;
    Elem* p= fila->head->prox;
    elem_deletado=p->value;
    fila->head->prox = p->prox;
    free(p);
    if(fila->head->prox==NULL){
        fila->last= fila->head;
    }
    return elem_deletado;
} 

void putBlockingQueue(BlockingQueue *Q,int i){
    pthread_mutex_lock(&myMutex);
    if(Q->statusBuffer==Q->sizeBuffer){
        printf("fila cheia!\n");
    }
    while(Q->statusBuffer==Q->sizeBuffer){
        pthread_cond_wait(&empty,&myMutex);
    }
    fila_push(Q,i);
    if(Q->statusBuffer==1) pthread_cond_broadcast(&fill);
    pthread_mutex_unlock(&myMutex);
}
int takeBlockingQueue(BlockingQueue* Q){
    pthread_mutex_lock(&myMutex);
    if(Q->statusBuffer==0){
        printf("fila vazia!\n");
    }
    while(Q->statusBuffer==0){
        pthread_cond_wait(&fill,&myMutex);
    }
    int result = fila_pop(Q);
    if(Q->statusBuffer==Q->sizeBuffer-1) pthread_cond_broadcast(&empty);
    pthread_mutex_unlock(&myMutex);
    return result;
}


BlockingQueue* newBlockingQueue(unsigned int SizeBuffer){
    BlockingQueue *aux = (BlockingQueue *)  malloc(sizeof(BlockingQueue));
    Elem *sentinela= (Elem *)   malloc(sizeof(Elem));
    sentinela->value=-1;
    sentinela->prox=NULL;
    aux->head=aux->last= sentinela; 
    aux->sizeBuffer= SizeBuffer;
    aux->statusBuffer=0;
    return aux;
}

void* producer(void* thread_fila){
    BlockingQueue *Q= (BlockingQueue *) thread_fila;
    printf("produtor!\n");
    for (int i = 0; 1; i++){
        putBlockingQueue(Q,i%200);
        printf("produzi %d\n", i%200);        
    }
    pthread_exit(NULL);
}

void* consumer(void* thread_fila){
    BlockingQueue *Q= (BlockingQueue *) thread_fila;
    printf("consumidor!\n");
    for (int i = 0; 1; i++){
        int res=takeBlockingQueue(Q);
        printf("consumir %d\n",res);
    }
    pthread_exit(NULL);
} 

int main(int qtd_argv, int** argvc){
    BlockingQueue* fila= newBlockingQueue(tam_da_fila);
    pthread_t threads_consumer[QTD_CONSUMER], threads_producer[QTD_PRODUCER];
    for (int i = 0; i < QTD_PRODUCER; i++){
        pthread_create(&threads_producer[i], NULL, producer,(void *) fila);
    }
    for (int i = 0; i < QTD_CONSUMER; i++){
        pthread_create(&threads_consumer[i], NULL, consumer,(void *) fila);
    }
    for (int i = 0; i < QTD_PRODUCER; i++){
        pthread_join(threads_producer[i], NULL);    
    }
    for (int i = 0; i < QTD_CONSUMER; i++){
        pthread_join(threads_consumer[i], NULL);    
    }
    pthread_exit(NULL); 
    return 0;
}