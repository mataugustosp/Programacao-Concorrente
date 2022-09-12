/*
 * Produtores e Consumidores.
 * Estudo Dirigido
 * Aluno: Matheus Augusto Silva Pinho
 * Matrícula: 18/0024906
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PR 2 // número de produtores
#define CN 3 // número de consumidores
#define N 5  // tamanho do buffer

void *produtor(void *arg);
void *consumidor(void *arg);

// Semaforos
sem_t *mutex;
sem_t *empty;
sem_t *full;
// variaveis
int buffer_data = 0, buffer[N], buffer_position = 0, data = 0;

int main(argc, argv) int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  // Declarando semáforos

  // Semáforo que ira garantir a exclusao mutua
  sem_unlink("mutex");
  mutex = sem_open("mutex", O_CREAT, 0777, 1);
  if (mutex == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore mutex");
    exit(EXIT_FAILURE);
  }
  // Conta os lugares vazios do buffer
  sem_unlink("empty");
  empty = sem_open("empty", O_CREAT, 0777, N);
  if (empty == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore empty");
    exit(EXIT_FAILURE);
  }
  // Conta os lugares cheios do buffer
  sem_unlink("full");
  full = sem_open("full", O_CREAT, 0777, 0);
  if (full == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore full");
    exit(EXIT_FAILURE);
  }

  // Criação das threads
  pthread_t tid[PR];

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
  pthread_join(tCid[0], NULL);
}

void *produtor(void *arg)
{
  int i = *((int *)arg);

  while (1)
  {
    // O produtor Consome a quantidade de lugares vazios N
    sem_wait(empty);
    sem_wait(mutex);

    /*** Zona critica***/
    buffer[buffer_position] = buffer_data;
    printf("PRODUTOR INSERIU: %d\n", buffer_data);
    buffer_position++;
    buffer_data++;
    sleep(3);
    /*** FIM Zona Critica ***/

    sem_post(mutex);
    // Produtor incrementa Quantidade de lugares cheios 
    sem_post(full);
  }
  pthread_exit(0);
}

void *consumidor(void *arg)
{
  int i = *((int *)arg);
  while (1)
  {
    // Consumidor consome a quantidade de lugares cheios
    sem_wait(full);
    sem_wait(mutex);

    /*** Zona critica***/
    buffer_position--;
    int data = buffer[buffer_position];
    printf("CONSUMIDOR: Data read: %d\n", data);
    sleep(3);
    /*** FIM Zona critica ***/

    sem_post(mutex);
    // Incrementa a quantidade de lugares vazios
    sem_post(empty);
  }
  pthread_exit(0);
}
