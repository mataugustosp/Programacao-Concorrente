//Estudo Dirigido 3
// Aluno: Matheus Augusto Silva Pinho
// Matrícula: 18/0024906
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 // macacos que andam de A para B
#define MB 10 // macacos que andam de B para A

pthread_mutex_t lock_AB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_BA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_corda = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_mutual = PTHREAD_MUTEX_INITIALIZER;

int m1 = 0, m2 = 0;

void *macacoAB(void *a)
{
  int i = *((int *)a);
  while (1)
  {
    pthread_mutex_lock(&lock_mutual);
    pthread_mutex_lock(&lock_AB);
    m1++;
    if (m1 == 1)
    {
      pthread_mutex_lock(&lock_corda);
    }
    pthread_mutex_unlock(&lock_AB);
    pthread_mutex_unlock(&lock_mutual);

    // Procedimentos para acessar a corda
    printf("Macaco %d passado de A para B \n", i);
    sleep(1);
    // Procedimentos para quando sair da corda
    pthread_mutex_lock(&lock_AB);
    m1--;
    if (m1 == 0)
    {
      pthread_mutex_unlock(&lock_corda);
    }
    pthread_mutex_unlock(&lock_AB);
  }
  pthread_exit(0);
}

void *macacoBA(void *a)
{
  int i = *((int *)a);
  while (1)
  {
    pthread_mutex_lock(&lock_mutual);
    pthread_mutex_lock(&lock_BA);
    m2++;
    if (m2 == 1)
    {
      pthread_mutex_lock(&lock_corda);
    }
    pthread_mutex_unlock(&lock_BA);
    pthread_mutex_unlock(&lock_mutual);
    // Procedimentos para acessar a corda
    printf("Macaco %d passado de B para A \n", i);
    sleep(1);
    // Procedimentos para quando sair da corda
    pthread_mutex_lock(&lock_BA);
    m2--;
    if (m2 == 0)
    {
      pthread_mutex_unlock(&lock_corda);
    }
    pthread_mutex_unlock(&lock_BA);
  }
  pthread_exit(0);
}

void *gorila(void *a)
{
  while (1)
  {
    pthread_mutex_lock(&lock_mutual);
    pthread_mutex_lock(&lock_corda);
    // Procedimentos para acessar a corda
    printf("Gorila passado de A para B \n");
    sleep(5);
    // Procedimentos para quando sair da corda
    pthread_mutex_unlock(&lock_corda);
    pthread_mutex_unlock(&lock_mutual);
  }
  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t macacos[MA + MB];
  int *id;
  int i = 0;

  for (i = 0; i < MA + MB; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    if (i % 2 == 0)
    {
      if (pthread_create(&macacos[i], NULL, &macacoAB, (void *)id))
      {
        printf("Não pode criar a thread %d\n", i);
        return -1;
      }
    }
    else
    {
      if (pthread_create(&macacos[i], NULL, &macacoBA, (void *)id))
      {
        printf("Não pode criar a thread %d\n", i);
        return -1;
      }
    }
  }
  pthread_t g;
  pthread_create(&g, NULL, &gorila, NULL);

  pthread_join(macacos[0], NULL);
  return 0;
}
