#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5

#define ESQ(id) (id + N - 1) % N
#define DIR(id) (id + 1) % N
/*
        Para N = 5, temos que, em uma mesa circular:
        Filósofo de id = 0: ESQ(0) = 4 e DIR(0) = 1
        Filósofo de id = 1: ESQ(1) = 0 e DIR(1) = 2
        Filósofo de id = 2: ESQ(2) = 1 e DIR(2) = 3
        Filósofo de id = 3: ESQ(3) = 2 e DIR(3) = 4
        Filósofo de id = 4: ESQ(4) = 3 e DIR(4) = 0

*/

// ESTADOS DOS FILÓSOFOS
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

int estados[N];

// Mutex para tornar as açoes de pegar talher e devolver talher atômicas
// podia também ser um semáforo
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Semáforos com permissões para filósofo seguir em frente ou não
sem_t s[N];

void *filosofos(void *arg);
void pega_talher(int n);
void devolve_talher(int n);
void come(int n);

int main()
{
    int i;
    int *id;
    // semaforo
    for (i = 0; i < N; i++)
    {
        sem_init(&s[i], 0, 1);
    }
    pthread_t r[N];

    // criacao das threads de filosofos
    for (i = 0; i < N; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&r[i], NULL, filosofos, (void *)(id));
    }

    pthread_join(r[0], NULL);
    return 0;
}

void *filosofos(void *arg)
{
    int n = *((int *)arg);
    while (1)
    {
        // pensar
        printf("Filosofo %d pensando ...\n", n);
        sleep(3);
        pega_talher(n);
        // comer
        printf("\tFilosofo %d comendo ...\n", n);
        sleep(3);
        printf("\tFilosofo %d acabou de comer ...\n", n);
        devolve_talher(n);
    }
}

void pega_talher(int n)
{
    // Bloqueia a mesa
    pthread_mutex_lock(&lock);

    estados[n] = FAMINTO;
    tenta_comer(n);

    pthread_mutex_unlock(&lock);
    // verifica se comeu para devolver os talheres. Se não tiver comido, trava
    sem_wait(&s[n]);
}

void devolve_talher(int n)
{
    // Bloqueia a mesa
    pthread_mutex_lock(&lock);
    estados[n] = PENSANDO;
    // verifica se tem algum dos filósofos ao seu lado querendo comer 
    // se tiver, ele manda comer 
    tenta_comer(ESQ[n]);
    tenta_comer(DIR[n]);

    pthread_mutex_unlock(&lock);
}

void tenta_comer(int n)
{
    if(estados[n] == FAMINTO && estados[ESQ(n)] != COMENDO && estados[DIR(n)] != COMENDO){
        estados[n] = COMENDO;
        sem_post(&s[n]);
    }
}

