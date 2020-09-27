#ifndef BIGBOOL_BIG_BOOL_H
#define BIGBOOL_BIG_BOOL_H

#include <stdint.h>

#define MAX_PARTS 20

typedef enum {false, true} bool;

typedef struct BigBool {
    uint8_t parts[MAX_PARTS];
    size_t last_part;
    size_t last_bit;
} BigBool;

BigBool* BB_from_string(char*);     // BigBool из строки
BigBool* BB_from_uint64(uint64_t);  // BigBool из uint64
BigBool* BB_copy(BigBool*);         // BigBool из BigBool (копирование)
BigBool* BB_random(int);            // Случайный BigBool
BigBool* BB_ones(size_t);           // BigBool, у которого первые n бит - единицы

char* BB_to_string(BigBool*);       // Строковое представление

bool BB_eq(BigBool*, BigBool*);     // Сравнение двух векторов

BigBool* BB_assign_not(BigBool*);               // Изменение bb на ~bb
BigBool* BB_assign_and(BigBool*, BigBool*);     // Изменение bb1 на bb1 & bb2
BigBool* BB_assign_or(BigBool*, BigBool*);      // Изменение bb1 на bb1 | bb2
BigBool* BB_assign_xor(BigBool*, BigBool*);     // Изменение bb1 на bb1 ^ bb2

BigBool* BB_not(BigBool*);              // Новый вектор ~bb
BigBool* BB_and(BigBool*, BigBool*);    // Новый вектор bb1 & bb2
BigBool* BB_or(BigBool*, BigBool*);     // Новый вектор bb1 | bb2
BigBool* BB_xor(BigBool*, BigBool*);    // Новый вектор bb1 ^ bb2

BigBool* BB_assign_shift_left(BigBool*, int);           // Сдвиг влево
BigBool* BB_assign_shift_right(BigBool*, int);          // Сдвиг вправо
BigBool* BB_assign_cycle_shift_left(BigBool*, int);     // Циклический сдвиг влево
BigBool* BB_assign_cycle_shift_right(BigBool*, int);    // Циклический сдвиг вправо

BigBool* BB_shift_left(BigBool*, int);          // Новый вектор, сдвинутый влево
BigBool* BB_shift_right(BigBool*, int);         // Новый вектор, сдвинутый вправо
BigBool* BB_cycle_shift_left(BigBool*, int);    // Новый вектор, циклически сдвинутый влево
BigBool* BB_cycle_shift_right(BigBool*, int);   // Новый вектор, циклически сдвинутый вправо

#endif //BIGBOOL_BIG_BOOL_H
