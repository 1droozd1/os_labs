#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void *search_in(char* pat, char* txt)
{
    int len_of_pattern = strlen(pat);

    printf("%d\n", len_of_pattern);

    int len_of_str = strlen(txt);

    printf("%s\n", txt);
    printf("%s\n", pat);

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

    int kol_in_txt;

    printf("amount in text = ");
    scanf("%d", &kol_in_txt);
    int lenght_of_pat = strlen(pat);


    char* text;
    text = (char *) malloc(kol_in_txt * sizeof(char));
    for (int i = 0; i < kol_in_txt; i++) {
        text[i] = "ABC"[random () % 3];
    }

    printf("%s\n", text);
    printf("%s\n", pat);

    search_in(pat, text);

    return 0;
}