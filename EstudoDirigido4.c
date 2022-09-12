//Estudo Dirigido 4
// Aluno: Matheus Augusto Silva Pinho
// Matrícula: 18/0024906

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PR 2 //número de produtores
#define CN 2 // número de consumidores
#define N 5  //tamanho do buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_cheio;
pthread_cond_t buffer_vazio;

int *buffer;
int buffer_data = 0;
int buffer_position = 0;

void *produtor(void *meuid);
void *consumidor(void *meuid);

void main(argc, argv) int argc;
char *argv[];
{

    int erro;
    int i, n, m;
    int *id;

    pthread_t tid[PR];
    buffer = (int *)malloc(N * sizeof(int));
    pthread_cond_init(&buffer_cheio, NULL);
    pthread_cond_init(&buffer_vazio, NULL);

    for (i = 0; i < PR; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        erro = pthread_create(&tid[i], NULL, produtor, (void *)(id));

        if (erro)
        {
            printf("erro na criacao do thread %d\n", i);
            exit(1);
        }
    }

    pthread_t tCid[CN];

    for (i = 0; i < CN; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        erro = pthread_create(&tCid[i], NULL, consumidor, (void *)(id));

        if (erro)
        {
            printf("erro na criacao do thread %d\n", i);
            exit(1);
        }
    }

    pthread_join(tid[0], NULL);
}

void *produtor(void *pi)
{

    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (buffer_position >= N)
        {
            pthread_cond_wait(&buffer_cheio, &mutex);
        }

        /*** Zona critica***/
        buffer[buffer_position] = buffer_data;
        printf("PRODUTOR INSERIU: %d\n", buffer_data);
        buffer_position++;
        buffer_data++;
        /*** FIM Zona Critica ***/

        pthread_cond_signal(&buffer_vazio);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void *consumidor(void *pi)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (buffer_position <= 0)
        {
            pthread_cond_wait(&buffer_vazio, &mutex);
        }

        /*** Zona critica***/
        buffer_position--;
        int data = buffer[buffer_position];
        printf("CONSUMIDOR: Data read: %d\n", data);
        /*** FIM Zona critica ***/

        pthread_cond_signal(&buffer_cheio);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}
