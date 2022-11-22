#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

typedef struct numbers
{
    int num;
    int st;
    char *file_name;
} numbers;

int human_get(sem_t *semaphore)
{
    int s;
    sem_getvalue(semaphore, &s);
    return s; 
}

void human_set(sem_t *semaphore, int n)
{
    while (human_get(semaphore) < n)
    {
        sem_post(semaphore);
    }
    while (human_get(semaphore) > n)
    {
        sem_wait(semaphore);
    }
}


int main(int args, char *argv[])
{
    pid_t id;
    int sum = 0, n = 0;
    numbers* mapped = (numbers*)mmap(0, sizeof(numbers), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    char file_name[20];

    if (mapped == MAP_FAILED)
    {
        printf("mmap error\n");
        return -1;
    }

    id = fork();
    sem_unlink("_sem");
    sem_t *sem = sem_open("_sem", O_CREAT, 0, 2);

    if (id < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (id == 0) {

        /*execlp ("./child_process", mapped, NULL);
        fprintf(stderr, "\nExec didn't work...\n");
        exit(1);*/
        char file_name_read[20] = "123";

        remove(file_name_read);

        FILE *write_res;
        if ((write_res = fopen(file_name_read, "w")) == NULL) {
            printf("Error: can't open file\n");
            exit(1);
        }

        while(1) 
        {
            while(human_get(sem) == 2)
            {
                continue;
            }
            if (mapped->st == 1) 
            {
                sum += mapped->num;
                fprintf(write_res, "%d", sum);
                sum = 0;
                human_set(sem, 2);
            }
            else if (mapped->st == 2)
            {
                sum += mapped->num;
                fprintf(write_res, "%d", sum);
                fclose(write_res);
                human_set(sem, 0);
                exit(0);
            }
            else if (mapped->st == 0)
            {
                sum += mapped->num;
                human_set(sem, 2);
            }
        }

    }
    //Parent process
    else {
        printf("[Parent Process, id=%d]: Write name of file: ", getpid());
        /*fgets(file_name, 20, stdin);
        if (file_name[strlen(file_name) - 1] == '\n')
            file_name[strlen(file_name) - 1] = '\0';

        mapped->file_name = (char *)file_name;*/

        while(human_get(sem) != 0) 
        {
            char c;
            scanf("%d%c", &n, &c);
            mapped->num = n;
            if (c == ' ')
            {
                mapped->st = 0;
            }
            if (c == '\n')
            {
                mapped->st = 1;
            }
            if (c == '\0')
            {
                mapped->st = 2;
            }
            human_set(sem, 1);
            while(human_get(sem) == 1)
            {
                continue;
            }
        }

    }


    munmap(mapped, sizeof(numbers));
    sem_close(sem);
    sem_destroy(sem);

    return 0;
}