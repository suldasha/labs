#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX_LEN 30
char *usage = "Флаги для аргументов командной строки:\n"\
                "-a - полный вывод ключей\n"
                "-s - поиск ключа по известному расшифрованному тексту\n"\
                "Пример:./ИМЯ_ПРОГРАММЫ ФЛАГ\n";

void attack(char *ciphertext, char *key, char *decrypt_text);  
void get_key(char *ciphertext, char *key, char *decrypt_text, int output, char *sample);

int main(int argc, char *argv[])
{
    // Проверка на правильную работу с флагами
    if(argc != 2 || (strcmp(argv[1], "-a") && strcmp(argv[1], "-s")))
    {
        fprintf(stderr, "Ошибка аргументов командной строки.\n");
        puts(usage);
        return 1;
    }

    // output - параметр для ф-ий расшифрования
    // 0 - поиск ключа по образцу
    // 1 - вывод всех ключей
    int output = 0;
    if (!strcmp(argv[1], "-a"))
        output = 1;

    // Выделение памяти и ввод шифртекста
    char *ciphertext = calloc(MAX_LEN, sizeof(char));
    if(ciphertext == NULL)
    {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        return -1;
    }

    puts("Введите шифртекст, зашифрованный ключем длины 5:");
    fgets(ciphertext, MAX_LEN, stdin);

    // Создаем нулевой указатель, не указывающий на конкретную область памяти
    // В случае поиска по образцу - выделяем память и вводим образец
    char *sample = NULL;
    if(!output)
    {
        sample = calloc(strlen(ciphertext) + 1, sizeof(char));
        if(sample == NULL)
        {
            fprintf(stderr, "Ошибка выделения памяти.\n");
            return -1;
        }

        printf("Введите образец: \n");
        fgets(sample, strlen(ciphertext) + 1, stdin);
    }

    // По условию ключ имеет длину 5
    char key[5];
    key[5]='\0';
    char *decrypt_text = calloc(strlen(ciphertext) + 1, sizeof(char)); 
    if(decrypt_text == NULL)
    {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        return -1;
    } 
    decrypt_text[strlen(ciphertext)] = '\0';
    get_key(ciphertext, key, decrypt_text, output, sample);

    free(ciphertext);
    free(decrypt_text);
    if(!output)
        free(sample);
    return 0;
}

// Функция переключения ключа
void get_key(char *ciphertext, char *key, char *decrypt_text, int output, char *sample)
{
    // Перебираем в цикле все возможные ключи длины 5
    for(int i0 ='a'; i0 < 'z'; i0++)
    {
        key[0] = i0;
        for(int i1 = 'a'; i1 < 'z'; i1++)
        {
            key[1] = i1;
            for(int i2 = 'a'; i2 < 'z'; i2++)
            {
                key[2] = i2;
                for(int i3 = 'a'; i3 < 'z'; i3++)
                {
                    key[3] = i3;
                    for(int i4 = 'a'; i4 < 'z'; i4++)
                    {
                        key[4] = i4;
                        attack(ciphertext, key, decrypt_text);
                        // Для -s
                        if(!output)
                        {
                            if(!strcmp(decrypt_text, sample))
                                printf("Ключ: %s\n", key);              
                        }
                        // Для -а
                        else
                        {
                            printf("Ключ: %s  Расшифровка: %s\n", key, decrypt_text);
                        }
                    }
                }
            }
        }
    }
}

void attack(char *ciphertext, char *key, char *decrypt_text) 
{
    int ptr_key = 0;
    for(int ptr_msg = 0; ptr_msg < strlen(ciphertext); ptr_msg++)        
    {
        // Обновляем ключ если дошли до его конца
        if(ptr_key == strlen(key)) 
            ptr_key = 0;

        // Если символ - не буква, записываем его в расшифрованный текст
        // без изменений для корректного шифрования
        if(!isalpha(ciphertext[ptr_msg]))
        {
            decrypt_text[ptr_msg] = ciphertext[ptr_msg];
            continue;
        }  

        // Учитываем регистр букв
        int low = 'a';
        if(!islower(ciphertext[ptr_msg])) 
            low = 'A';

        // Приводим символы к числовому виду
        char al_msg = ciphertext[ptr_msg] - low;
        char al_key = key[ptr_key] - 'a'; 
        // Дешифруем по формуле (с + N + k)%N = d, приводим к символу и переключаем ключ         
        decrypt_text[ptr_msg]=((al_msg + 26 - al_key) % 26) + low; 
        ptr_key++;
    }
}

