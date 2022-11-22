#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int main(int argc, char const *argv[])
{
    char read_file_name[] = argv[0];
    int str_len  = atoi(argv[1]);

    printf("[Child Process,  id=%d]", getpid());

    int memFd = shm_open(read_file_name, O_RDONLY, 0);
    if (memFd == -1) {
        perror("Can't open file");
        return 1;
    }
    void *buffer = mmap(NULL, str_len, PROT_READ, MAP_SHARED, memFd, 0);
    if (buffer == NULL) {
        perror("Can't mmap");
        return -1;
    }

    char *str_ptr = malloc(str_len);
    int i = 0;

    fscanf(read_file_name, "%s", str_ptr);

    printf("%s\n", str_ptr); 

    int res = sum_from_char(str_ptr);
    free(str_ptr);

    fprintf(read_file_name, "%d", res);

    //??
    if (munmap(buffer, str_len) == -1) {
        perror("munmao");
        return 1;
    }

    return 0;
}