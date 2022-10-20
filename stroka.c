#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LEN 80

int main()
{
    char *s = calloc(MAX_LEN, sizeof(char));
    char *w = calloc(MAX_LEN, sizeof(char));
    if(s == NULL)
    {
        fprintf(stderr, "Error memory allocate\n");
        return -1;
    }
    if(w == NULL)
    {
        fprintf(stderr, "Error memory allocate\n");
        return -1;
    }

    puts("Enter a string:" );
    fgets(s, MAX_LEN, stdin);
    puts("Enter a world:" );
    fgets(w, MAX_LEN, stdin);

    char *ptr = strtok(s," ");
    int k = 0;
    while (ptr)
    {
        if(strcmp(ptr,w) == 0)
        {
            k++;
        }
        ptr = strtok(NULL," ");
    }
    printf("Number of repeats:%d",k);

    free(ptr);
    free(w);
    free(s);
    return 0;
}