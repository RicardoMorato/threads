#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define qtd_threads 4
#define fim 1000000
int contador=0;
int n;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
void *contabilizador(void* thread){
    int *id= (int *)thread;
    
    while(1){   //concorrência das threads, utilizamos mutex
        pthread_mutex_lock(&mymutex);   // bloquia mutex
        if(contador==fim){
            pthread_mutex_unlock(&mymutex);  //caso uma thread chegue em 1000000, liberamos mutex, e saímos do while
            break;
        }
        contador++;
        if(contador==fim){  // a thread que alcançõu 1000000, printa que ganhou, libera o mutex,sai do laço 
            printf("A thread %d ganhouu, contador: %d\n",*id,contador);
            pthread_mutex_unlock(&mymutex);
            break;
        }
        pthread_mutex_unlock(&mymutex);   //libera mutex
        
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    printf("digite a qtd de threads: ");
    scanf("%d", &n); 
    pthread_t *threads;   //declarando threads
    threads= (pthread_t *) malloc(n*sizeof(pthread_t));
    int *identificador;
    identificador= (int *) malloc(n*sizeof(int));
    int rc;
    int flag=1;
    for (int i = 0; i < n; i++){
        identificador[i]=i+1;   // pômos um identificador para cada thread, para saber qual ganhou
        rc= pthread_create(&threads[i],NULL,contabilizador,(void *) &identificador[i]);   //criando todas as threads
        if(rc){   // só entra se der erro na
            printf("ocorreu um erro na criacao de alguma thread!!\n");
            exit(-1);
        }
    } 
    for (int i = 0; i < qtd_threads; i++){
        pthread_join(threads[i],NULL);    //aguardamos todas as threads acabarem
    }
    
    
    pthread_exit(NULL);
    return 0;
}