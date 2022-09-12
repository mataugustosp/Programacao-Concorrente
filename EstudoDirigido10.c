#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

#define CARTAS 20 //quantidade de cartas na mochila

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pombo = PTHREAD_COND_INITIALIZER;
pthread_cond_t usuarios = PTHREAD_COND_INITIALIZER;

int mochila = 0;

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;

    pthread_t usuario[N];
    int *id;
    for(i = 0; i < N; i++){
         id = (int *) malloc(sizeof(int));
          *id = i;
	pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
    }
     pthread_t pombo;
     id = (int *) malloc(sizeof(int));
     *id = 0;
     pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

     pthread_join(pombo,NULL);
   
}


void * f_pombo(void *arg){
  
    while(1){
        pthread_lock(&mutex);
        //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        if(mochila < CARTAS )
        {
            pthread_cond_wait(&pombo,&mutex);
        }
        //Leva as cartas para B e volta para A
        mochila=0;
        //Acordar os usuários   
        pthread_cond_broadcast(&usuarios);

        pthread_unlock(&mutex);
    }
}

void * f_usuario(void *arg){
    
    while(1){
	    //Escreve uma carta
        sleep(2);
        pthread_lock(&mutex);
        //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme	
        while(mochila == CARTAS )
        {
            pthread_cond_wait(&usuarios,&mutex);
        }
        //Posta sua carta na mochila do pombo
        mochila++;
        //Caso a mochila fique cheia, acorda o ṕombo
        if(mochila == CARTAS)
        {
            pthread_cond_signal(&pombo);
        }
        pthread_unlock(&mutex);
    }
}
