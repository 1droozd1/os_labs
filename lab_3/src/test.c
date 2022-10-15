#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

typedef struct thread_data {

    int a;
    int b;
    int result;

} thread_data;

void *myThreadfunction(void *args)
{
    thread_data *tdata = (thread_data *)args;
    int a = tdata -> a;
    int b = tdata -> b;
    int result = a + b;

    tdata -> result = result;
    pthread_exit(NULL);
}


int main()
{
    pthread_t th[8];
    thread_data tdata;

    tdata.a = 1;
    tdata.b = 2;

    int massive[8];
    
    pthread_create(&th[0],NULL, myThreadfunction, (void *)&tdata);
    pthread_join(th[0], NULL);

    printf("Thread resilt = %d\n", tdata.result);

    return 1;
}