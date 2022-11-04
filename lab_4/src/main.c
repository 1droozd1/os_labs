#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "semaphore.h"

typedef struct numbers
{
    int num;
    int st;
} numbers;




int main(int args, char *argv[])
{
    pid_t id;
    int sum = 0;
    numbers* mapped = (numbers*)mmap(0, sizeof(numbers), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    char file_name[20];
    char sequence_of_numbers[100];

    if (mapped == MAP_FAILED)
    {
        printf("mmap error\n");
        return -1;
    }

    id = fork();

    if (id < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (id == 0) {
    

    }
    //Parent process
    else {
        printf("[Parent Process, id=%d]: Write name of file: ", getpid());
        fgets(file_name, 20, stdin);
        if (file_name[strlen(file_name) - 1] == '\n')
            file_name[strlen(file_name) - 1] = '\0';

    }



    return 0;
}