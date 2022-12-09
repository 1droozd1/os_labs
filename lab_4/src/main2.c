#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct number{

    int num;
    int st;
    char filename[20];

} number;

int getting_value(sem_t *semaphore)
{
    int s;
    sem_getvalue(semaphore, &s);
    return s; 
}

void setting_value(sem_t *semaphore, int n)
{
    while (getting_value(semaphore) < n)
    {
        sem_post(semaphore);
    }
    while (getting_value(semaphore) > n)
    {
        sem_wait(semaphore);
    }
}

int main(int args, char *argv[])
{
    pid_t id;
    char file_name[20];

    number *buffer = mmap(NULL, sizeof(number), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (buffer == NULL) {
        perror("Can't mmap");
        return -1;
    }

    sem_t *semaphore = mmap(NULL, sizeof(sem_t), PROT_READ |PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    sem_init(semaphore, 1, 2);

    id = fork();

    if (id < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (id == 0) {
        int sum = 0;
        while(1) 
        {
            while(getting_value(semaphore) == 2)
            {
                continue;
            }
            if (buffer->st == 0) 
            {
                sum += buffer->num;
                setting_value(semaphore, 2);
            }
            else if (buffer->st == 1)
            {
                sum += buffer->num;

                remove(buffer->filename);

                FILE *write_res;
                if ((write_res = fopen(buffer->filename, "w")) == NULL) {
                    printf("Error: can't open file\n");
                    exit(1);
                }
                fprintf(write_res, "%d", sum);
                buffer->num = sum;
                fclose(write_res);

                setting_value(semaphore, 0);
                exit(0);
            }
        }

    }
    //Parent process
    else {
        printf("[Parent Process, id=%d]: Write name of file: ", getpid());
        fgets(file_name, 20, stdin);
        if (file_name[strlen(file_name) - 1] == '\n')
            file_name[strlen(file_name) - 1] = '\0';
        
        strcpy(buffer->filename, file_name);

        printf("[Parent Process, id=%d]: Write int numbers: ", getpid());
        
        while(getting_value(semaphore) != 0) 
        {
            char c;
            scanf("%d%c", &buffer->num, &c);
            if (c == ' ')
            {
                buffer->st = 0;
            }
            if (c == '\n')
            {
                buffer->st = 1;
            }
            setting_value(semaphore, 1);
            while(getting_value(semaphore) == 1)
            {
                continue;
            }
        }
        printf("[Parent Process, id=%d]: Result: %d\n", getpid(), buffer->num);
    }

    if (munmap(buffer, sizeof(number))!= 0) {
        printf("UnMapping Failed\n");
        return 1;
    }
    sem_destroy(semaphore);

    if (munmap(semaphore, sizeof(semaphore))!= 0) {
        printf("UnMapping of semaphore failed\n");
        return 1;
    }
    return 0;
}