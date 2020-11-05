#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define tam_x 3
int p,n;  // n é a quantidade de threads
//double A[2][2]={{2,1},{5,7}};
double A[3][3]={{8,1,1},{1,7,2},{2,1,9}};   
//double A[1][1]={{2}};
//double B[2]={11,13};
double B[3]={13,21,31};
//double B[1]={12};
double x_k_mais_1[tam_x];
double x_k[tam_x]={1,1,1};
pthread_barrier_t barreira;

void print(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("%lf ", A[i][j]);
        }
        printf("\n");
    }
}
void print_p(){
    for (int i = 0; i < tam_x; i++){
        printf("%.2lf ",x_k_mais_1[i]);
    }
    printf("\n");
}
void* jacobi(void* thread){
    int *id= (int*)thread;
    int k=1;
    while(k <=p){
        for (int i = *id; i < tam_x; i+=n){  // algorítmo de jacobi
            double aux=0;
            for (int j = 0; j < tam_x; j++){   //varre pelas colunas de A
                if(j!=i){
                    aux+=A[i][j]*x_k[j];
                }
            }
            x_k_mais_1[i]= (1/A[i][i])* (B[i]- aux);
            
        }
        pthread_barrier_wait(&barreira);    //espera as outras threads calcular seus x_mais_1
        for (int i = *id; i < tam_x; i+=n){
            x_k[i]=x_k_mais_1[i];    // atualiza os x antigos
        }
        k++;
        //barreira espera
        pthread_barrier_wait(&barreira);   // só vai pra próxima interação dps q tds as threads acabarem
    }
    pthread_exit(NULL);
}


int main(){
    printf("digite o quantidade de threads: ");
    scanf("%d", &n);
    printf("digite o p: ");
    scanf("%d", &p);
    pthread_barrier_init(&barreira,NULL, n);

    pthread_t *threads=(pthread_t *)  malloc(n*sizeof(pthread_t));
    int *thread_id= (int* ) malloc(n*sizeof(int));
    int rc;
    
    for (int i = 0; i < n; i++){
        thread_id[i]=i;
        rc= pthread_create(&threads[i],NULL,jacobi,(void *) &thread_id[i]); 
        if(rc){
            printf("erro na criacao de uma thread!\n");
            exit(-1);
        }
    }
    for (int i = 0; i < n; i++){
        pthread_join(threads[i],NULL);
    }
    
    
    pthread_barrier_destroy(&barreira);
    
    print_p();
    
    pthread_exit(NULL);
    return 0;
}