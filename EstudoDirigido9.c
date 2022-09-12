#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 // número de usuários

#define CARTAS 20 // quantidade de cartas na mochila

// Declaração dos semáforos

sem_t *s_pombo;
sem_t *s_cartas; 
sem_t *escreve_carta;

int mochila = 0; 

void *f_usuario(void *arg);
void *f_pombo(void *arg);

int main(int argc, char **argv)
{
    int i;

    // Inicializando semáforos
    sem_unlink("s_pombo");
    s_pombo = sem_open("s_pombo", O_CREAT, 0777, 0);
    if (s_pombo == SEM_FAILED)
    {
        fprintf(stderr, "%s\n", "ERROR creating semaphore s_pombo");
        exit(EXIT_FAILURE);
    }

    sem_unlink("s_cartas");
    s_cartas = sem_open("s_cartas", O_CREAT, 0777, CARTAS);
    if (s_cartas == SEM_FAILED)
    {
        fprintf(stderr, "%s\n", "ERROR creating semaphore s_cartas");
        exit(EXIT_FAILURE);
    }

    sem_unlink("mutex");
    mutex = sem_open("mutex", O_CREAT, 0777, 1);
    if (mutex == SEM_FAILED)
    {
        fprintf(stderr, "%s\n", "ERROR creating semaphore mutex");
        exit(EXIT_FAILURE);
    }

    pthread_t usuario[N];
    int *id;
    for (i = 0; i < N; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&(usuario[i]), NULL, f_usuario, (void *)(id));
    }
    pthread_t pombo;
    id = (int *)malloc(sizeof(int));
    *id = 0;
    pthread_create(&(pombo), NULL, f_pombo, (void *)(id));

    pthread_join(pombo, NULL);
}

void *f_pombo(void *arg)
{
    printf("Entrou Pombo");
    while (1)
    {
        
        // Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        sem_wait(s_pombo);
        // Leva as cartas para B e volta para A
        printf("O pombo está levando as cartas de B para A");
        mochila = 0;
        // Acordar os usuários
        for(int i = 0; i < CARTAS; i++)
        {
            sem_post(s_cartas);
        }
    }
}

void *f_usuario(void *arg)
{
    int myid = *(int *)(arg);
    while (1)
    {
        // Escreve uma carta
        sleep(2);
        // Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme 
        sem_wait(s_cartas);
        // Posta sua carta na mochila do pombo
        sem_wait(mutex);
        mochila++;
        printf("Usuário %d escreveu carta", myid);
         sleep(1);
        // Caso a mochila fique cheia, acorda o ṕombo
        if(mochila == CARTAS)
        {
            sem_post(s_pombo);
        }
        sem_post(mutex);
    }
}
