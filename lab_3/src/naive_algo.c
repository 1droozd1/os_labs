void *search_in(char* pat, char* txt)
{
    int len_of_pattern = strlen(pat);
    int len_of_str = strlen(txt);
 
    for (int i = 0; i <= len_of_str - len_of_pattern; i++) {
        int j;
        for (j = 0; j < len_of_pattern; j++) {
            if (txt[i + j] != pat[j])
                break;
            if (j == len_of_pattern)
            printf("Pattern found at index %d \n", i);
        }
    }
}