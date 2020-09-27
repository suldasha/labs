#include <string.h>
#include <stdio.h>

int main()
{
char vse[80];
char horoso[80];
int n = 0; 
char *blin; 

puts("Введите предложение:");
gets(vse);

puts("Введите слово:");
gets(horoso);

blin = vse;

while ( blin = strstr(blin, horoso) ) 
{
n++;
blin++; // перейти на следующий символ для сравнения
}

printf("В строке: ");
puts(vse); 
printf("Слово: ");
puts(horoso); 
printf("Встречается %d раз", n);

getchar();

return 0;
}