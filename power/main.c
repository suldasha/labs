#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <ctype.h>

typedef int32_t (*callback)(uint32_t);
extern int32_t power(int32_t num, uint32_t pow, callback cb);
char sign = ' ';//глобальная, чтобы не передавать в несколько функций

int check_correct(char* str_num, char* str_pow){

    int i = 0;
    //проверка на отрицательное основание степени
    if (str_num[i] == '-'){
        i++;
        sign = '-';
    }
    
    //проверки на корректность введенных данных
    while (str_num[i] != '\0'){
        if (!(isdigit(str_num[i]))){
            fprintf(stderr, "Основание степени должно быть целым числом.\n");
            return 1;
        } 
        i++;
    } 

    i = 0;
        while (str_pow[i] != '\0'){
        if (!(isdigit(str_pow[i]))){
            fprintf(stderr, "Показатель степени должен быть целым положительным числом.\n");
            return 1;
        } 
        i++;
    } 

    return 0;
}

int power_call(uint32_t num, uint32_t pow, callback cb){

    //константные случаи
    if(pow == 0){
        printf("%c%d в степени %d = 1\n", sign, num, pow);
        return 0;
    }

    if(num == 1 || num == -1){
        printf("%c%d в степени %d =%c1\n", sign, num, pow, sign);
        return 0;
    }

    //вычисление и вывод степени
    printf("%c%d в степени %d =%c", sign, num, pow, sign);
    if(power(num, pow, cb) == 1)
        return 1;
    
    return 0;
}

int32_t print_result(uint32_t res){
    //проверка на переполнение
    if(res == 1){
        fprintf(stderr, "\nРезультат слишком велик.\n");
        return 1;
    }

    printf("%d\n", res);
    return 0;

}

int main(int argc, char* argv[]){

    if(argc != 3){
        fprintf(stderr, "Передайте аргументами командной строки:%s ЧИСЛО СТЕПЕНЬ\n", argv[0]);
        return 1;
    }

    char* str_num = argv[1];
    char* str_pow = argv[2];

    if(check_correct(str_num, str_pow)){
        return 1;
    }

    uint32_t num = (uint32_t)abs(strtol(str_num, NULL, 10));
    if (errno){
        fprintf(stderr, "Основание степени слишком велико.\n");
        return 1;
    }

    uint32_t pow = (uint32_t)strtol(str_pow, NULL, 10);
    if (errno){
        fprintf(stderr, "Показатель степени слишком велик.\n");
        return 1;
    }
    
    //степень с четным показателем - всегда положительное число
    if(pow % 2 == 0)
        sign = ' ';

    if(power_call(num, pow, print_result))
        return 1;

    return 0;
}