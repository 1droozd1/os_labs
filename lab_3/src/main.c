#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "string.h"

typedef struct thread_data {

    char *pat;
    char *txt;
    int i;

} thread_data;


void *threads_searching(void* args)
{
    thread_data *tdata = (thread_data *)args;

    char *pat = tdata -> pat;
    char *txt = tdata -> txt;
    int i = tdata -> i;

    int len_of_pattern = strlen(pat);
    int len_of_str = strlen(txt);

    int j;
    for (j = 0; j < len_of_pattern; j++) {
        if (txt[i + j] != pat[j])
            break;
        if (j == len_of_pattern) {
            printf("Pattern found at index %d \n", i);
        }
    }

    free(tdata);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) 
{
    pthread_t *th;
    int threads_amount = atoi(argv[0]);

    if (threads_amount < 2) {
        printf("Write amount of threads: ");
        scanf("%d", &threads_amount);
    }
    printf("Amount of threads = %d\n", threads_amount);

    int len_txt;
    printf("Write amount of symbols in text: ");
    scanf("%d", &len_txt);

    char *text;
    text = (char*) malloc(len_txt * sizeof(char));

    for (int i = 0; i < len_txt; i++) {
        char randomletter = "ABC"[random () % 3];
        text[i] = randomletter;
    }

    printf("text = %s\n", text);

    char pat[20];
    printf("Write pattern - ");
    scanf("%s", pat);

    int lenght_pat = strlen(pat);

    while (lenght_pat > len_txt) {
        printf("Wrong pattern, write pattern less than a string: \n");
        scanf("%s", pat);
        printf("Pattern = %s\n", pat);
    }

    int max_threads_amount = len_txt / lenght_pat;

    if (threads_amount > max_threads_amount) {
        threads_amount = max_threads_amount;
        printf("Too many threads, max amount of threads is %d\n", threads_amount);
    }

    th = (pthread_t *) malloc(threads_amount * sizeof(double));
    
    if (th == NULL) {
        printf("Error with threads\n");
    }

    int kolSym_in_str = max_threads_amount;
    char *strok;

    for (int i = 0; i < len_txt - lenght_pat; i += kolSym_in_str) {

        thread_data *tdata = malloc(sizeof(thread_data));

        printf("3\n");

        tdata -> pat = (char *)&pat;

        strok = (char*) malloc(kolSym_in_str * sizeof(char));
        strncpy(strok, (char *) &text[i] , kolSym_in_str);

        tdata -> txt = (char *)&strok;
        tdata -> i = i;

        if ((pthread_create(&th[i], NULL, threads_searching, (void *)&tdata)) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < len_txt - lenght_pat; i++) {
        if ((pthread_join(th[i], NULL)) != 0) {
            perror("Failed to join thread");
        }
    }

    free(strok);
    strok = NULL;
    free(text);
    text = NULL;
    free(th); 
    th = NULL;

    return 0;
}