#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXSIZE 10000  

pthread_barrier_t barrier;

int size;		
int soma_parcial[MAXSIZE] = {};	
int soma_total = 0;				 

int matrix[MAXSIZE][MAXSIZE];

void *Worker(void *);

int main(int argc, char *argv[]) {
	int i, j;
	
	if(argc != 2){  
		printf("Numero de argumentos invalido! Deve ser igual a 2 (nome do programa e tamanho da matriz).\n");
		return 0;
	}
	
    size = atoi(argv[1]);	
  
    if(size > MAXSIZE){		
	    printf("Tamanho muito grande!\n");
        return 1;	
    }

    pthread_t workerid[size];
  
    pthread_barrier_init(&barrier, NULL, size);

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            matrix[i][j] = 1;		

    int * id;

    for (i = 0; i < size; i++){
        id = (int *) malloc(sizeof(int));
        *id = i;
		pthread_create(&workerid[i], NULL, Worker, (void *) (id));
    }

    for (i = 0; i < size; i++){
        if(pthread_join(workerid[i], NULL)){
            printf("\n ERROR joining thread");
            exit(1);
        }
    }

    printf("Bye!\n");
}


void *Worker(void *arg) {
	int myid = *(int *)(arg);
	int j, k;
	int self = pthread_self();
 
	printf("worker %d (pthread id %d) has started\n", myid, self);

	for(k = 0; k < size; k++){
		soma_parcial[myid] += matrix[myid][k];	
	}

    pthread_barrier_wait(&barrier);		
	
    if (myid == 0) {	
        printf("\n ");
		printf("Matriz criada:");
		printf("\n ");
        for(j = 0; j < size; j++){
			soma_total += soma_parcial[j];	
	        for(k = 0; k < size; k++){
                printf("%d ",matrix[j][k]);   
            }
            printf("\n ");
        }
		printf("\nSoma total = %d.\n", soma_total);	
}


void read_data_base(int i)
{
	printf("Leitor %d está lendo os dados! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5);
}

void use_data_read(int i)
{
	printf("Leitor %d está usando os dados lidos! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5);
}

void think_up_data(int i)
{
	printf("Escritor %d está pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i)
{
	printf("Escritor %d está escrevendo os dados! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5 + 15);
}
