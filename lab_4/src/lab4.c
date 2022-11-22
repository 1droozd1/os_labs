#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME file_forLab.txt

const unsigned int MAX_LENGTH = 1 * 1024 * 1024;  // Не выделять больше мегабайта памяти
const unsigned int CHUNK_SIZE = 1024; 

int main(int argc, char const *argv[])
{
    unsigned int str_len = CHUNK_SIZE;
    char *str_ptr = malloc(CHUNK_SIZE);           // Выделяем первый килобайтный блок

    if (str_ptr == NULL)
        err(EXIT_FAILURE, "Не удалось выделить память!\n");
    


    pid_t id;
    char filename[] = "FILENAME";

    id = fork();
    // error: can't create child process
    if (id < 0) {
        perror("fork error");
        return -1;
    } else if (id == 0) { //child process
        char str_lenght[20];
        snprintf(str_lenght, sizeof(str_lenght), "%d", str_len);
        execlp ("./child_process", filename, str_lenght, NULL);
        fprintf(stderr, "\nExec didn't work...\n");
        exit(1);
    } else { //parent process

        printf("[Parent Process, id=%d]", getpid());
        printf("[Parent Process, id=%d]: Write int numbers: ", getpid());
        
        int c;
        unsigned int i;
        for (i = 0, c = EOF; (c = getchar()) != '\n' && c != EOF; i++) {
            str_ptr[i] = c;

            if (i == MAX_LENGTH) {
                free(str_ptr);
                err(EXIT_FAILURE, "Слишком много входных данных!\n");
            }
        
            if (i == str_len) {                       // Блок заполнен
                str_len = i + CHUNK_SIZE;
                str_ptr = realloc(str_ptr, str_len);  // Расширяем блок на ещё один килобайт
            }
        }
        str_ptr[i] = '\0';                          // Признак конца строки

        int memFd = shm_open(filename, O_CREAT | O_RDWR, S_IRWXU);
        if (memFd == -1) {
        perror("Can't open file");
        return 1;
        }

        int result;

        int res = ftruncate(memFd, str_len + 1);
        if (res == -1) {
            perror("Can't truncate file");
            return res;
        }
        void *buffer = mmap(NULL, str_len + 1, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
        if (buffer == NULL) {
            perror("Can't mmap");
            return -1;
        }
        fprintf(buffer, "%s", str_ptr);

        free(str_ptr);

        /// waiting a miracle

        fscanf(filename, "%d", result);

        if (munmap(buffer, str_len) == -1) {
            perror("munmao");
            return 1;
        }

        printf("[Parent Process, id=%d] Finall result is %d", getpid(), result);

    }
    return 0;
}