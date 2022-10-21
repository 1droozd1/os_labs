#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main()
{
    pid_t id;
    int pipe1[2];
    char file_name[20];
    char sequence_of_numbers[100];

    // error: can't create pipe
    if (pipe(pipe1) == -1) {
        printf("Unable to create pipe\n");
        return 1;
    }
    
    id = fork();
    // error: can't create child process
    if (id < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (id == 0) {
        
        char pipe_1[20], pipe_2[20];
        snprintf(pipe_1, sizeof(pipe_1), "%d", pipe1[0]); //transform int to string format
        snprintf(pipe_2, sizeof(pipe_2), "%d", pipe1[1]);

        execlp ("./child_process", pipe_1, pipe_2, NULL);
        fprintf(stderr, "\nExec didn't work...\n");
        exit(1);
    }
    // Parent Process
    else {
        printf("[Parent Process, id=%d]: Write name of file: ", getpid());
        fgets(file_name, 20, stdin);
        if (file_name[strlen(file_name) - 1] == '\n')
            file_name[strlen(file_name) - 1] = '\0';
        
        printf("[Parent Process, id=%d]: Write int numbers: ", getpid());   
        fgets(sequence_of_numbers, 100, stdin);
        
        if (sequence_of_numbers[strlen(sequence_of_numbers) - 1] == '\n')
            sequence_of_numbers[strlen(sequence_of_numbers) - 1] = '\0';
        printf("[Parent Process, id=%d]: File name: %s\n\n", getpid(), file_name);


        close(pipe1[0]);

        write(pipe1[1], file_name, sizeof(file_name));
        write(pipe1[1], sequence_of_numbers, sizeof(sequence_of_numbers));

        close(pipe1[0]);
        close(pipe1[1]);
    }
    return 0;
}
