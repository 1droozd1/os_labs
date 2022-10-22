#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"
#include "time.h"

//valgrind --tool=memcheck ./a.out
//strace -f -e 'clone' ./a.out

typedef struct thread_data {

    long int size;
    long int kol_sym;
    char *pat;
    char *txt;

} thread_data;


void *threads_searching(void* args)
{
    thread_data *tdata = (thread_data *)args;

    char *pattern = tdata -> pat;
    long int count = tdata -> size;
    long int size = tdata -> kol_sym;
    char *text = tdata -> txt;

    char *string_from_text;
    string_from_text = (char *) malloc((size + 1) * sizeof(char));
    strncpy(string_from_text, (char *) &text[count], size);
    string_from_text[size] = '\0';


    int len_of_pattern = strlen(pattern);
 
    for (int i = 0; i < size - len_of_pattern; i++) {
        for (int j = 0; j < len_of_pattern; j++) {
            if (string_from_text[i + j] != pattern[j]) {
                break;
            }
            if (j == len_of_pattern - 1) {
                printf("Pattern found at index: %ld\n", count + 1);
            }
        }
    }
    
    free(tdata);
    free(string_from_text);
    return NULL;
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

    char pat[20];
    printf("Write pattern: ");
    scanf("%s", pat);

    int lenght_pat = strlen(pat);

    while (lenght_pat > len_txt) {
        printf("Wrong pattern, write pattern less than a string: \n");
        scanf("%s", pat);
        printf("Pattern = %s\n", pat);
    }

    clock_t t1, t2;
 
    t1 = clock();

    int max_threads_amount = len_txt / lenght_pat;

    if (threads_amount > max_threads_amount) {
        threads_amount = max_threads_amount;
        printf("Too many threads, max amount of threads is %d\n", threads_amount);
    }

    th = (pthread_t *) malloc(threads_amount * sizeof(pthread_t));
    
    if (th == NULL) {
        printf("Error with threads\n");
    }

    int kolSym_in_str = (len_txt / threads_amount);
    char *strok;

    int flag1 = 0;

    if (len_txt % threads_amount != 0) {
        flag1 = 1;
    }

    int j = 0;

    for (int i = 0; i < threads_amount; i++) {

        thread_data *tdata = malloc(sizeof(thread_data));
        
        if (flag1 == 1) { // the number of text characters is not 
                          //divisible by the number of threads

            if (i == (threads_amount - 1)) {

                tdata -> kol_sym = kolSym_in_str + (len_txt % threads_amount) + 1;
                
            } else {

                tdata -> kol_sym = kolSym_in_str + lenght_pat;

            }

        } else { //the number of characters divided by the number of threads (without modulo)

            if (i != (threads_amount - 1)) {

                tdata -> kol_sym = kolSym_in_str + lenght_pat - 1;

            } else {
                tdata -> kol_sym = kolSym_in_str;
            }
        }

        tdata -> pat = (char *)pat;
        tdata -> txt = (char *)text;
        tdata -> size = j;

        j += kolSym_in_str;

        if ((pthread_create(&th[i], NULL, threads_searching, (void *)tdata)) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < threads_amount; i++) {
        if ((pthread_join(th[i], NULL)) != 0) {
            perror("Failed to join thread");
        }
    }

    free(text);
    free(th);

    t2 = clock();

    printf("Сompilation time: %f\n", (t2 - t1) / (double)CLOCKS_PER_SEC);
    
    return 0;
}