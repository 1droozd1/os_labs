#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

void *search_in(char* pat, char* txt)
{
    int len_of_pattern = strlen(pat);

    printf("%d\n", len_of_pattern);

    int len_of_str = strlen(txt);

    int flag1 = 0;
 
    for (int i = 0; i < len_of_str - len_of_pattern; i++) {
        for (int j = 0; j < len_of_pattern; j++) {
            if (txt[i + j] != pat[j]) {
                break;
            }
            if (j == len_of_pattern - 1) {
                printf("Pattern found at index %d \n", i);
                flag1 = 1;
            }
        }
    }

    if (flag1 == 0) {
        printf("No pattern\n");
    }
}

int main()
{
    char pat[20];
    printf("pat = ");
    scanf("%s", pat);
    clock_t t1, t2;

    int kol_in_txt;

    printf("amount in text = ");
    scanf("%d", &kol_in_txt);
    int lenght_of_pat = strlen(pat);


    char* text;
    text = (char *) malloc(kol_in_txt * sizeof(char));
    for (int i = 0; i < kol_in_txt; i++) {
        text[i] = "ABC"[random () % 3];
    }
    
    printf("%s\n", pat);
    printf("%s\n", text);

    t1 = clock();

    search_in(pat, text);

    t2 = clock();

    printf("Compilation time: %f\n", (t2 - t1) /  (double)CLOCKS_PER_SEC);

    return 0;
}