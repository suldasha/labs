//подсчитать сколько раз слово W встречается в предложении P

#include<stdio.h>
#include<conio.h>
#include<string.h>

int main()
{
    char *s = calloc (1, sizeof(char));
    char *w = calloc (1, sizeof(char));
    char *ptr = calloc (1, sizeof(char))
    int k=0;
    
    puts("Enter a string:" );
    gets(s);
    puts("Enter a world:" );
    gets(w);
    
    ptr=strtok(s," ");
    while (ptr)
    {
        if(strcmp(ptr,w)==0)
        {
            k++;
        }
        ptr=strtok(NULL," ");  

    }
    printf("Number of repeats:%d",k);
    
    free(ptr);
    free(w);
    free(s);
    return 0; 
}