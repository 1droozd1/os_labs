#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

const unsigned int MAX_LENGTH = 1 * 1024 * 1024;  // Не выделять больше мегабайта памяти
const unsigned int CHUNK_SIZE = 100;

typedef struct number{
    int num;
    int result;
    int st;
    char* read_num;
    char* filename;
} number;

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

int sum_from_char(char *s)
{
    int i = 0, sum = 0, n = 0, flag1 = 0;
    int len = strlen(s);

    for (int i = 0; i < len; ++i) {
        if (s[i] >= '0' && s[i] <= '9' && flag1 == 0) {
            n = n * 10 + (s[i] - '0');
        } else if (flag1 == 1 && s[i] >= '0' && s[i] <= '9') {
            n = n * 10 - (s[i] - '0');
        } else if (s[i] == '-') {
            flag1 = 1;
        } else if (s[i] == ' ' && flag1 == 1) {
            flag1 = 0;
            sum += n;
            n = 0;
            continue;
        } else if (n) {
            sum += n;
            n = 0;
            flag1 = 0;
        }
    }
    return sum + n;
}


int main(int args, char *argv[])
{
    pid_t id;
    unsigned int str_len = CHUNK_SIZE;
    char *str_ptr = malloc(CHUNK_SIZE * sizeof(char));
    number *buffer;
    char file_name[20];

    sem_unlink("_sem");
    sem_t *semaphore = sem_open("_sem", O_CREAT, 0, 2);
    printf("%d\n", human_get(semaphore));

    id = fork();

    if (id < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (id == 0) {

        while(1) {
            while(human_get(semaphore) == 2)
            {
                continue;
            }
            
            printf("something\n");
            char read_file_name[20];
            printf("%d\n", human_get(semaphore));

            printf("%s\n", buffer->filename);
            strcpy(read_file_name, buffer->filename);
            printf("second\n");

            char* read_sequence_of_numbers;
            read_sequence_of_numbers = (char*)malloc(sizeof(char) * (buffer->num));
            strcpy(read_sequence_of_numbers, buffer->read_num);

            printf("[Child Process,  id=%d]: File name from the pipe: %s\n", getpid(), read_file_name);
            printf("[Child Process,  id=%d]: Numbers from the pipe: %s\n", getpid(), read_sequence_of_numbers);

            remove(read_file_name); //if we have the same file

            buffer->result = sum_from_char(read_sequence_of_numbers);

            free(read_sequence_of_numbers);

            FILE *write_res;
            if ((write_res = fopen(read_file_name, "w")) == NULL) {
                printf("Error: can't open file\n");
                exit(1);
            }
            printf("Результат: %d\n", buffer->result);
            fprintf(write_res, "%d", buffer->result);

            human_set(semaphore, 0);
            fclose(write_res);
            exit(0);
        }
        

    }
    //Parent process
    else {
        while(human_get(semaphore) != 0) {
            printf("[Parent Process, id=%d]: Write name of file: ", getpid());
            fgets(file_name, 20, stdin);
            if (file_name[strlen(file_name) - 1] == '\n')
                file_name[strlen(file_name) - 1] = '\0';

            printf("[Parent Process, id=%d]: Write int numbers: ", getpid());
            int c;
            unsigned int i;
        
            for (i = 0, c = EOF; (c = getchar()) != '\n' && c != EOF; i++) {
                str_ptr[i] = c;

                if (i == MAX_LENGTH) {
                    free(str_ptr);
                    printf("Слишком много входных данных!\n");
                    exit(1);
                }
        
                if (i == str_len) {                         // Блок заполнен
                    str_len = i + CHUNK_SIZE;
                    str_ptr = realloc(str_ptr, str_len);    // Расширяем блок на ещё один килобайт
                }
            }
            str_ptr[i] = '\0';                          // Признак конца строки

            printf("%d\n", str_len + 1);
            printf("%s\n", str_ptr);

            buffer = mmap(NULL, sizeof(number), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
            if (buffer == NULL) {
                perror("Can't mmap");
                return -1;
            }

            buffer->num = str_len;
            buffer->filename = (char*) file_name;
            buffer->read_num = (char*) str_ptr;

            printf("%d\n", buffer->num);
            printf("%s\n", buffer->read_num);

            human_set(semaphore, 1);
            printf("%d\n", human_get(semaphore));
            while(human_get(semaphore) == 1)
            {
                continue;
            }
        }

        printf("[Parent Process, id=%d] Result: %d\n", getpid(), buffer->result);
    }

    free(str_ptr);

    if (munmap(buffer, sizeof(number))!= 0) {
        printf("UnMapping Failed\n");
        return 1;
    }
    sem_close(semaphore);
    sem_destroy(semaphore);

    return 0;
}