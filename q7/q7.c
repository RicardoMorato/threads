#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

#define THREADS 1

typedef struct Elem{
   int R, G, B;
}Elem;

int **mat;
Elem **pixel;
int i, j, LIN, COL;

void *conversao(void * info) {
    int* id = (int*) info;
    for(int i = *id; i < (LIN * COL); i+=THREADS){
        int k = i/COL, j = i%COL;
        mat[k][j] = (int) (pixel[k][j].R*0.30 + pixel[k][j].G*0.59 + pixel[k][j].B*0.11);
    }
    pthread_exit(NULL);
}

int main () {
    pthread_t thread[THREADS];
    FILE *img;
    char texto[20];
    int status, *threadID, max;

    // abrindo a imagem
    img = fopen("img.ppm", "r");
    if(img == NULL) {
        printf("Erro na abertura do arquivo!");
        return 1;
    }

    // Leitura das dimensões
    fgets(texto, 20, img);
    fscanf(img, "%d %d", &COL, &LIN);
    fscanf(img, "%d", &max);

    // Alocaçao do array
    mat = (int **) malloc(LIN * sizeof(int *));
    for (i=0; i < LIN; i++)
        mat[i] = (int *) malloc(COL * sizeof(int));

    
    // Alocaçao do array
    threadID = (int *) malloc(THREADS * sizeof(int));

    pixel = (Elem **) malloc(LIN * sizeof(Elem));
    for (i=0; i < LIN; i++)
        pixel[i] = (Elem*) malloc(COL * sizeof(Elem));
    
    //Conversao
    for(i = 0; i < LIN; i++) {
        for(j = 0; j < COL; j++){
            fscanf(img, "%d %d %d", &pixel[i][j].R, &pixel[i][j].G, &pixel[i][j].B);
        }
    }

    for (i = 0; i < THREADS; i++){
        threadID[i] = i; 
        status = pthread_create(&thread[i],NULL,conversao,(void *) &threadID[i]); 
        if(status){
            printf("Erro ao criar Thread!");
            exit(-1);
        }
    }
    
    for(i = 0; i < THREADS; i++) {
        pthread_join(thread[i],NULL);
    }

    // gerando nova imagem
    img = fopen("new-image.ppm", "w");
    if(img == NULL) {
        printf("Erro na criaçao do arquivo!");
        return 1;
    }

    fprintf(img, "P3\n%d %d\n%d\n", COL, LIN, max);
    for(i = 0; i < LIN; i++) {
        for(j = 0; j < COL; j++){
            int aux = mat[i][j];
            fprintf(img, "%d %d %d\n", aux, aux, aux); 
        }
    }
    
    for(i = 0; i < LIN; i++){
        free(mat[i]);
        free(pixel[i]);
    }
    free(mat);
    free(pixel);

    fclose(img);
    return 0;
}
