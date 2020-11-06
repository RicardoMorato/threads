#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#define tam 100
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
int qtd_thread;
int qtd_substrings=0;
typedef struct{
    char *frase;
    int len_frase;
}String_x;

typedef struct{
    String_x s1;
    String_x s2;
    int id;
}Pacote;
void inicializa_string(String_x *s, int n){
    s->len_frase=n;
    s->frase= (char *)  malloc((n+1)*sizeof(char));
    scanf("%s", s->frase);
} 
void quantidade_substring(String_x s1, String_x s2, int id){
    pthread_mutex_lock(&mutex);
    int n1= s1.len_frase, n2= s2.len_frase;
    for (int i = id; i < n1-n2+1; i+=qtd_thread){
        int flag=1;
        for (int j = 0; j < n2; j++){
            if(s1.frase[i+j]!= s2.frase[j]) flag=0;
        }
        if(flag==1){
            qtd_substrings++;
        } 
    }
    pthread_mutex_unlock(&mutex);
}
void *calcula_substring(void* argumentos){
    Pacote* pacote= (Pacote *) argumentos;
    //printf("recebi id: %d, s1: %s, s2: %s!\n", pacote->id,pacote->s1.frase,pacote->s2.frase);
    quantidade_substring(pacote->s1,pacote->s2,pacote->id);
    pthread_exit(NULL);
}

int main(){
    pthread_t *threads;
    String_x s1, s2;
    int n1,n2;
    scanf("%d",&n1);
    inicializa_string(&s1, n1);
    scanf("%d",&n2);
    inicializa_string(&s2, n2);
    scanf("%d", &qtd_thread);
    
    threads= (pthread_t *) malloc(qtd_thread* sizeof(pthread_t));
    int rc;
    Pacote* pacote= (Pacote*) malloc(qtd_thread* sizeof(Pacote));;
    for (int i = 0; i < qtd_thread; i++){
        pacote[i].s1=s1; pacote[i].s2=s2; 
        pacote[i].id=i;
        rc= pthread_create(&threads[i], NULL, calcula_substring, &pacote[i]);
        if(rc){
            printf("Erro ao criar thread %d!\n", i);
        }
    }
    for (int i = 0; i < qtd_thread; i++){
        pthread_join(threads[i], NULL);
    }
    free(pacote);
    printf("%d\n", qtd_substrings);
    
    pthread_exit(NULL);
    
    return 0;
}
