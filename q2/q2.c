#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//L o número de linhas, TAM_L a largura das linhas
//N o número de arquivos
//T o número de threads

/*         Acrescentar linhas    */
//Mude o valor L e coloque as cores desejadas no vetor cores

/*       Acrescentar arquivos        */
//Mude o valor de N e coloque os arquivos desejados no vetor arquivos
// O NOME DAS CIDADES NÃO DEVEM CONTER ESPAÇOS
#define L 7
#define TAM_L 26
#define N 8
#define T 4

char *arquivos[8] = {"teste1.txt",
                     "teste2.txt",
                     "teste3.txt",
                     "teste4.txt",
                     "teste5.txt",
                     "teste6.txt",
                     "teste7.txt",
                     "teste8.txt"};
char tabela[L][TAM_L + 1];
char *cores[L] = {"\033[48;2;255;0;0",
                  "\033[48;2;255;255;0",
                  "\033[48;2;0;0;255",
                  "\033[48;2;255;0;255",
                  "\033[48;2;0;255;0",
                  "\033[48;2;0;0;0",
                  "\033[48;2;0;186;186"};
char *preto = ";30m";
char *branco = "m";

pthread_mutex_t mutex_linhas[L], mutex_cursor = PTHREAD_MUTEX_INITIALIZER;
int ids[T];

void inicia_tabela()
{
    int i, j;
    for (i = 0; i < L; i++)
    {
        for (j = 0; j < TAM_L; j++)
        {
            tabela[i][j] = ' ';
        }
        tabela[i][j] = '\0';
    }
    system("clear");
    for (int i = 0; i < L; i++)
    {
        if (i == L - 2)
        {
            printf("%s%s%s\033[m\n", cores[i], branco, tabela[i]);
        }
        else
        {
            printf("%s%s%s\033[m\n", cores[i], preto, tabela[i]);
        }
    }
    printf("\033[7A");
    system("sleep 2");
}

void limpa_linha(int l)
{
    int i;
    for (i = 0; i < TAM_L; i++)
    {
        tabela[l][i] = ' ';
    }
}

void muda_linha(int l, char codigo[], char cidade[], char hora[])
{
    int i, j = 0;
    for (i = 0; codigo[i] != '\0'; i++)
    {
        tabela[l][j] = codigo[i];
        j++;
    }
    j++;
    for (i = 0; cidade[i] != '\0'; i++)
    {
        tabela[l][j] = cidade[i];
        j++;
    }
    j = TAM_L - 5;
    for (i = 0; hora[i] != '\0'; i++)
    {
        tabela[l][j] = hora[i];
        j++;
    }
}

void *atualiza_tabela(void *tid)
{
    FILE *arq;
    char codigo[11], cidade[11], hora[6];
    int result, l, i;
    int *id = (int *)tid;
    for (i = *id; i < N; i += T)
    {
        arq = fopen(arquivos[i], "rt");
        if (arq == NULL)
        {
            fprintf(stderr, "Falha ao abrir arquivo!\n");
            exit(-1);
        }
        while (!feof(arq))
        {
            result = fscanf(arq, "%d %s %s %s", &l, codigo, cidade, hora);
            pthread_mutex_lock(&mutex_linhas[l - 1]);
            if (result != EOF)
            {
                limpa_linha(l - 1);
                muda_linha(l - 1, codigo, cidade, hora);
                pthread_mutex_lock(&mutex_cursor);
                if (l != 1)
                {
                    printf("\033[%dB", l - 1);
                }
                if (l == 6)
                {
                    printf("%s%s%s\033[m\n", cores[l - 1], branco, tabela[l - 1]);
                }
                else
                {
                    printf("%s%s%s\033[m\n", cores[l - 1], preto, tabela[l - 1]);
                }
                printf("\033[%dA", l);
                pthread_mutex_unlock(&mutex_cursor);
                system("sleep 2");
            }
            pthread_mutex_unlock(&mutex_linhas[l - 1]);
        }
        fclose(arq);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[T];
    int i;
    inicia_tabela();
    for (i = 0; i < L; i++)
    {
        pthread_mutex_init(&mutex_linhas[i], NULL);
    }
    for (i = 0; i < T; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, atualiza_tabela, (void *)&ids[i]);
    }
    for (i = 0; i < T; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for (i = 0; i < L; i++)
    {
        pthread_mutex_destroy(&mutex_linhas[i]);
    }
    printf("\033[7B\n");
    pthread_exit(NULL);
}
