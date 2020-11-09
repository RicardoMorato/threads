#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define QTD_CONSUMER 3
#define QTD_PRODUCER 3

const int tam_da_fila=10;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

//Fizemos uma fila em vez de um vetor para representar a produção e consumo no buffer, como pedido na questão 
typedef struct elem{   //Nó da fila
   int value;
   struct elem *prox;
}Elem;
 
typedef struct blockingQueue{  //fila
   unsigned int sizeBuffer, statusBuffer;   // sizeBuffer==tamanho máximo da fila, statusBuffer= tamanho atual da fila
   Elem *head,*last;
}BlockingQueue;

void teste(BlockingQueue* fila){  // função para teste
    printf("elem_head: %d, elem_last: %d tam_total: %d,  tamanho_atual: %d\n", fila->head->value, fila->last->value, fila->sizeBuffer, fila->statusBuffer);
}

void printa_fila(BlockingQueue *fila){    // função para teste
    Elem* cur= fila->head;
    while(cur->prox!= NULL){
        printf("%d ", cur->prox->value);
        cur= cur->prox;
    }
    printf("\n");
    printf("valor_head: %d, valor_last: %d\n", fila->head->prox->value, fila->last->value);
}
void fila_push(BlockingQueue *fila, int valor){  //função para inserir um nó na fila
    fila->statusBuffer++;    //aumento o tamanho atual
    Elem* cauda= fila->last;      // cauda é um cursor que aponta para o último elemento da fila
    Elem* novo = (Elem *)  malloc(sizeof(Elem));
    novo->value= valor;    novo->prox= NULL;
    cauda->prox= novo;
    fila->last= novo;     // crio um novo nó e o próximo do último nó aponta para o novo nó
}
int fila_pop(BlockingQueue *fila){  //função para remover um elemento da fila
    if(fila->head->prox==NULL){   // Se a fila tiver vazia, ñ removo(precaução apenas)
        printf("fila vazia!!\n");
        return -1;
    }
    int elem_deletado=0;
    fila->statusBuffer--;   //diminuo o tamanho atual
    Elem* p= fila->head->prox;
    elem_deletado=p->value;   //aponto para o elemento que quero remover
    fila->head->prox = p->prox;
    free(p);   //apago ele;
    if(fila->head->prox==NULL){  // se eu apagar todos elemento, meu último elemento aponta para o head(aponta para o sentinela, que é apenas um nó físico que ponho)
        fila->last= fila->head;   // faço isso para não perder a referência do meu last
    }
    return elem_deletado;
} 

void putBlockingQueue(BlockingQueue *Q,int i){  //recebo minha fila e o elemento que vou colocar
    pthread_mutex_lock(&myMutex);   //travo o mutex(concorrência)
    if(Q->statusBuffer==Q->sizeBuffer){  //o que a questão pede para printar, que é quando a fila tá cheia
        printf("fila cheia!\n");
    }
    while(Q->statusBuffer==Q->sizeBuffer){   //se tiver cheio, boto pra dormir minha thread produtora
        pthread_cond_wait(&empty,&myMutex);
    }
    fila_push(Q,i);  //insiro elemento na fila
    if(Q->statusBuffer==1) pthread_cond_broadcast(&fill);  // se produzi um elemento, acordo todas threads consumidoras(pois podem estarem durmindo)
    //depois que acordar todas a primeira que consegui travar o mutex é a que vai operar
    pthread_mutex_unlock(&myMutex);  //distravo o mutex
}
int takeBlockingQueue(BlockingQueue* Q){   //retiro um elemento da fila
    pthread_mutex_lock(&myMutex);   //travo o mutex
    if(Q->statusBuffer==0){    // printo se a fila encheu (a questão pede)
        printf("fila vazia!\n");
    }
    while(Q->statusBuffer==0){   // enquanto tiver vazia a fila, eu ponho a thread consumidora para dormir
        pthread_cond_wait(&fill,&myMutex);
    }
    int result = fila_pop(Q);   //retiro o elemento da fila
    if(Q->statusBuffer==Q->sizeBuffer-1) pthread_cond_broadcast(&empty); // se estava cheia a fila e acabei de consumir 1, provavelmente as threads produtoras estavam dormindo
    //acordo todas as threads produtoras, a primeira que conseguir travar o mutex, é a que opera
    pthread_mutex_unlock(&myMutex);  // destravo o mutex
    return result;
}


BlockingQueue* newBlockingQueue(unsigned int SizeBuffer){   //inicializa minha fila
    BlockingQueue *aux = (BlockingQueue *)  malloc(sizeof(BlockingQueue));
    Elem *sentinela= (Elem *)   malloc(sizeof(Elem)); //crio um sentinela para inicialmente o head e last apontar para ele(apenas forma de implementar uma fila)
    sentinela->value=-1;  // sentinela é apenas um nó físico
    sentinela->prox=NULL;  // utilizo NULL para saber os final da fila 
    aux->head=aux->last= sentinela; 
    aux->sizeBuffer= SizeBuffer;
    aux->statusBuffer=0;
    return aux;   //retorno minha fila criada
}

void* producer(void* thread_fila){   //rotina que as threads prdutoras operam
    BlockingQueue *Q= (BlockingQueue *) thread_fila;   // pego minha fila passada como argumento
    printf("produtor!\n");
    for (int i = 0; 1; i++){   //loop infinito
        putBlockingQueue(Q,i%200);   // coloco o elemento que eu quero produzir em mod 200, devido ao limite do int
        printf("produzi %d\n", i%200);
    }
    pthread_exit(NULL);
}

void* consumer(void* thread_fila){   //rotina que as threads consumidoras operam
    BlockingQueue *Q= (BlockingQueue *) thread_fila;   // pego minha fila passada como argumento
    printf("consumidor!\n");
    for (int i = 0; 1; i++){   //loop infinito
        int res=takeBlockingQueue(Q);  // chama a função que pega um elemento  
        printf("consumir %d\n",res);
    }
    pthread_exit(NULL);
} 

int main(int qtd_argv, int** argvc){
    BlockingQueue* fila= newBlockingQueue(tam_da_fila);   //crio uma fila e mando para as rotinas das threads como parâmetro
    pthread_t threads_consumer[QTD_CONSUMER], threads_producer[QTD_PRODUCER];
    for (int i = 0; i < QTD_PRODUCER; i++){   // crio as threads produtoras
        pthread_create(&threads_producer[i], NULL, producer,(void *) fila);
    }
    for (int i = 0; i < QTD_CONSUMER; i++){   // crio as threads consumidoras
        pthread_create(&threads_consumer[i], NULL, consumer,(void *) fila);
    }
    for (int i = 0; i < QTD_PRODUCER; i++){   // aguardo as threads produtoras finalizarem
        pthread_join(threads_producer[i], NULL);    
    }
    for (int i = 0; i < QTD_CONSUMER; i++){   // aguardo as threads consumidoras finalizarem
        pthread_join(threads_consumer[i], NULL);    
    }
    pthread_exit(NULL); 
    return 0;
}