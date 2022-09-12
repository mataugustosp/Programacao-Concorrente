/*
 * Problema do barbeiro dorminhoco.
 */ 
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 50
#define N_CADEIRAS 5
 

sem_t *sem_cadeiras;
sem_t *barb;
sem_t *acorda;
sem_t *cliente_aguarda;

void * f_barbeiro(void *v) {

  while(1) {
    //...Esperar/dormindo algum cliente sentar na cadeira do barbeiro (e acordar o barbeiro)
    sem_wait(acorda);
    sleep(1); //Cortar o cabelo do cliente 
    printf("Barbeiro cortou o cabelo de um cliente\n"); 
    
    //...Liberar/desbloquear o cliente
    sem_post(cliente_aguarda);
  }
  pthread_exit(0);
}

void* f_cliente(void* v) {
  int id = *(int*) v;
  sleep(id%3);
  // O sem_trywait retorna um valor EAGAIN se o semáforo for bloqueado, dessa forma,
  // se retornar 0, é pq passou 

  // Da o wait na cadeira 
  if(sem_trywait(sem_cadeiras) == 0){ //conseguiu pegar uma cadeira de espera
     printf("Cliente %d entrou na barbearia \n",id);
     //... pegar/sentar a cadeira do barbeiro
     sem_wait(barb);
     //... liberar a sua cadeira de espera
     sem_post(sem_cadeiras);
     //... acordar o barbeiro para cortar seu cabelo
     sem_post(acorda);
     //... aguardar o corte do seu cabelo 
     // Cliente se bloqueia para esperar o corte 
     sem_wait(cliente_aguarda);
     //... liberar a cadeira do barbeiro
     sem_post(barb);
     printf("Cliente %d cortou o cabelo e foi embora \n",id);

  }
  else{//barbearia cheia
	printf("Barbearia cheia, cliente %d indo embora\n",id);
  }

  pthread_exit(0);
}

int main() {
  pthread_t thr_clientes[N_CLIENTES], thr_barbeiro;
  int i, id[N_CLIENTES];

  // Declara semáforos 
  sem_unlink("sem_cadeiras");
  sem_cadeiras = sem_open("sem_cadeiras", O_CREAT, 0777, N_CADEIRAS);
  if (sem_cadeiras == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore sem_cadeiras");
    exit(EXIT_FAILURE);
  }
  
  // Cadeira do barbeiro 
  sem_unlink("barb");
  barb = sem_open("barb", O_CREAT, 0777, 1);
  if (barb == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore barb");
    exit(EXIT_FAILURE);
  }
  // Semáforo para acordar o barbeiro 
  sem_unlink("acorda");
  acorda = sem_open("acorda", O_CREAT, 0777, 0);
  if (acorda == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore acorda");
    exit(EXIT_FAILURE);
  }

  sem_unlink("cliente_aguarda");
  cliente_aguarda = sem_open("cliente_aguarda", O_CREAT, 0777, 0);
  if (cliente_aguarda == SEM_FAILED)
  {
    fprintf(stderr, "%s\n", "ERROR creating semaphore cliente_aguarda");
    exit(EXIT_FAILURE);
  }
  
  for (i = 0; i < N_CLIENTES; i++) {
    id[i] = i;
    pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id[i]);
  }

  pthread_create(&thr_barbeiro, NULL, f_barbeiro, NULL);
  
  for (i = 0; i < N_CLIENTES; i++) 
    pthread_join(thr_clientes[i], NULL);

  /* Barbeiro assassinado */
  
  return 0;
}

