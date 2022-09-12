
#define PAC 10 
#define PBD 5

pthread_barrier_t barrier;

void * func_pac(void * arg)
{
    int myid = *(int *) (arg);
    while(1)
    {
        printf("A");
        pthread_barrier_wait(&barrier); // destrava a impressão do B


        pthread_barrier_wait(&barrier); // Trava para imprimir o C
        printf("C");
        pthread_barrier_wait(&barrier); // Libera a imnpressão do D
        pthread_barrier_wait(&barrier); // Trava pra imprimir o A novamente 
    }
}

void * func_pac(void * arg)
{
    int myid = *(int *) (arg);
    while(1)
    {
        pthread_barrier_wait(&barrier); // Trava para imprimir o A
        printf("B");
        pthread_barrier_wait(&barrier); // Libera a impressão do C

        pthread_barrier_wait(&barrier); // Trava para imprimir o D 
        printf("D");

        if( myid == 0){
            printf("/n");
            sleep(5);
        }
        pthread_barrier_wait(&barrier); // Libera a impressão do A 
    }
}


// usamos pac + pbd, pois é neste momento que as threads são liberadas 
pthread_barrier_init(&barrier, NULL, PBD + PAC);


