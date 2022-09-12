//Estudo Dirigido 5
// Aluno: Matheus Augusto Silva Pinho
// Matrícula: 18/0024906

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20

pthread_mutex_t caldeirao = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t caldeirao_vazio;
pthread_cond_t precisa_cozinhar;

int quantidade_caldeirao = 0;

void *canibal(void*meuid);
void *cozinheiro(int m);


void main(argc, argv)
int argc;
char *argv[];
{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[MAXCANIBAIS];
  pthread_cond_init(&caldeirao_vazio, NULL);
  pthread_cond_init(&precisa_cozinhar, NULL);

  if(argc != 3){
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  n = atoi (argv[1]); //número de canibais
  m = atoi (argv[2]); // quantidade de porções que o cozinheiro consegue preparar por vez
  printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

  if(n > MAXCANIBAIS){
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < n; i++)  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(m);
}

void * canibal (void* pi){
  
  while(1) {
    //Pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
    //Pega o caldeirão 
    pthread_mutex_lock(&caldeirao);

    while(quantidade_caldeirao == 0){
        //Acorda o cozinheiro para fazer a comida
        pthread_cond_signal(&precisa_cozinhar);
        //Faz o canibal aguardar 
        pthread_cond_wait(&caldeirao_vazio, &caldeirao);
    }

    printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
    quantidade_caldeirao--;
    // solta o caldeirão 
    pthread_mutex_unlock(&caldeirao);

    sleep(10);
  }
  
}

void *cozinheiro (int m){
 
  while(1){ 
        //Pega o caldeirão 

    pthread_mutex_lock(&caldeirao);
    if(quantidade_caldeirao != 0){
        //Espera o momento de precisar cozinhar
        pthread_cond_wait(&precisa_cozinhar, &caldeirao);
    }

    //dormir enquanto tiver comida
    printf("cozinheiro: vou cozinhar\n");
    quantidade_caldeirao += m;

    // O broadcast libera todos os canibais
    pthread_cond_broadcast(&caldeirao_vazio);
    // solta o caldeirão
    pthread_mutex_unlock(&caldeirao);

    sleep(20);
    //acordar os canibais
   }

}