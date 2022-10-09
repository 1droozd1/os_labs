#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "naive_algo.h"
#define THREAD_MAX 4

int main() 
{
    char text [100];
    for (int i = 0; i < 100; i++) {
        char randomletter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random () % 26];
        text[i] = randomletter;
    }
    printf("text = %s\n", text);

    char buf[20];
    printf("Write pattern - ");
    scanf("%s", buf);
    printf("buf = %s\n", buf);

    clock_t t1, t2;
    pthread_t threads[THREAD_MAX];




    return 0;
}