
void search_in(int i, int a, char* pat, char* txt)
{
    for (int j = 0; j < a; j++) {
            if (txt[i + j] != pat[j])
                break;
            if (j == a) {
                printf("Pattern found at index %d \n", i);
            }  
}

void search(char* pat, char* txt) //pattern and text
{
    int a = strlen(pat);
    int b = strlen(txt);

    for (int i = 0; i <= b - a; i++) {
        search_in(i, a, pat, txt);
    }
}