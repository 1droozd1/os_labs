#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int sum_from_char(char *s)
{
    int len = sizeof(s)/sizeof(char);
    int sum = 0, n = 0;

    for (int i = 0; i < (len + 1); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            n = n * 10 + (s[i] - '0');
        } else if (n) { // complete number
            sum += n;
            n = 0;
        }
    }
    return sum + n;
}

int main(int argc, char const *argv[])
{
    int pipe1[2];
    char read_file_name[20];
    char read_sequence_of_numbers[100];

    pipe1[0] = atoi(argv[0]);
    pipe1[1] = atoi(argv[1]);

    close(pipe1[1]);
    
    read(pipe1[0], &read_file_name, sizeof(read_file_name));
    read(pipe1[0], &read_sequence_of_numbers, sizeof(read_sequence_of_numbers));

    printf("[Child Process,  id=%d]: File name from the pipe: %s\n", getpid(), read_file_name);
    printf("[Child Process,  id=%d]: Numbers from the pipe: %s\n", getpid(), read_sequence_of_numbers);


    remove(read_file_name); //if we have the same file

    int res = sum_from_char(read_sequence_of_numbers);

    FILE *write_res;
    if ((write_res = fopen(read_file_name, "w")) == NULL) {
        printf("Error: can't open file\n");
        exit(1);
    }
        
    fprintf(write_res, "%d", res);
    fclose(write_res);

    close(pipe1[0]);
    close(pipe1[1]);
    return 0;
}