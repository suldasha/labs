#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "big_bool.h"
#include "tests.h"

#define VEC_MAX_LEN (8 * MAX_PARTS)
#define STR_MAX_LEN (8 * MAX_PARTS + 1)

const char file_vectors[] = "test_files/vectors.txt";
const char file_numbers[] = "test_files/numbers.txt";
const char file_not[] = "test_files/not.txt";
const char file_and[] = "test_files/and.txt";
const char file_or[] = "test_files/or.txt";
const char file_xor[] = "test_files/xor.txt";
const char file_shift_left[] = "test_files/shift_l.txt";
const char file_shift_right[] = "test_files/shift_r.txt";
const char file_cycle_shift_left[] = "test_files/shift_cl.txt";
const char file_cycle_shift_right[] = "test_files/shift_cr.txt";



void BB_test_all(int num_random_tests) {
    BB_test_not_random();
    BB_test_random(num_random_tests);
}

void BB_test_not_random() {
    BB_test_str_bb_str();
    BB_test_uint64_bb_uint64();
    BB_test_ones();

    BB_test_eq();
    BB_test_copy();

    BB_test_not();
    BB_test_and();
    BB_test_or();
    BB_test_xor();

    BB_test_shift_left();
    BB_test_shift_right();
    BB_test_cycle_shift_left();
    BB_test_cycle_shift_right();
}

void BB_test_random(int num_tests) {
    BB_test_random_eq_self(num_tests);
    BB_test_random_and(num_tests);
    BB_test_random_or(num_tests);
    BB_test_random_xor(num_tests);
    BB_test_random_xor_from_and_or(num_tests);
    BB_test_de_morgan(num_tests);
}

/*********** CREATE ************/

void BB_test_str_bb_str() {
    int num_tests;
    char str1[STR_MAX_LEN], *str2;
    BigBool *bb;

    FILE* fp = fopen(file_vectors, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_vectors);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp, "%s", str1);
        bb = BB_from_string(str1);
        str2 = BB_to_string(bb);

        assert(strcmp(str1, str2) == 0);

        free(bb);
        free(str2);
    }

    fclose(fp);
}

void BB_test_uint64_bb_uint64() {
    int num_tests;
    uint64_t n1, n2;
    BigBool *bb;
    char *str, *end;

    FILE* fp = fopen(file_numbers, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_numbers);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp, "%lld", &n1);
        bb = BB_from_uint64(n1);
        str = BB_to_string(bb);
        n2 = strtoull(str, &end, 2);

        assert(n1 == n2);

        free(bb);
        free(str);
    }

    fclose(fp);
}

void BB_test_ones() {
    char *str;
    BigBool *bb;

    bb = BB_ones(0);
    str = BB_to_string(bb);

    assert(strcmp(str, "0") == 0);

    free(bb);
    free(str);

    for (int i = 1; i <= VEC_MAX_LEN; ++i) {
        bb = BB_ones(i);
        str = BB_to_string(bb);

        assert(strlen(str) == i);

        for (int j = 0; j < i; ++j)
            assert(str[j] == '1');

        free(bb);
        free(str);
    }
}

void BB_test_eq() {
    int array_len;
    char **str_array;
    BigBool *bb1, *bb2;

    FILE* fp = fopen(file_vectors, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_vectors);
        return;
    }

    fscanf(fp, "%d", &array_len);
    str_array = malloc(array_len * sizeof(char*));

    for (int i = 0; i < array_len; ++i) {
        str_array[i] = malloc(STR_MAX_LEN);
        fscanf(fp, "%s", str_array[i]);
    }

    fclose(fp);

    for (int i = 0; i < array_len; ++i) {
        bb1 = BB_from_string(str_array[i]);

        for (int j = i; j < array_len; ++j) {
            bb2 = BB_from_string(str_array[j]);

            if (strcmp(str_array[i], str_array[j])) {
                assert(!BB_eq(bb1, bb2));
                assert(!BB_eq(bb2, bb1));
            } else {
                assert(BB_eq(bb1, bb2));
                assert(BB_eq(bb2, bb1));
            }

            free(bb2);
        }

        free(bb1);
    }

    for (int i = 0; i < array_len; ++i)
        free(str_array[i]);
    free(str_array);
}

void BB_test_copy() {
    int num_tests;
    char str[STR_MAX_LEN], *str1, *str2;
    BigBool *bb1, *bb2;

    FILE* fp = fopen(file_vectors, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_vectors);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp, "%s", str);
        bb1 = BB_from_string(str);
        bb2 = BB_copy(bb1);
        str1 = BB_to_string(bb1);
        str2 = BB_to_string(bb2);

        assert(BB_eq(bb1, bb2));
        assert(strcmp(str1, str2) == 0);

        free(bb1);
        free(bb2);
        free(str1);
        free(str2);
    }

    fclose(fp);
}

/********** NOT, AND, OR, XOR **********/

void BB_test_not() {
    int num_tests;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
    BigBool *bb1, *nbb1, *bb2, *nbb2;

    FILE* fp = fopen(file_not, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_not);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp, "%s %s", str1, str2);

        bb1 = BB_from_string(str1);
        nbb1 = BB_not(bb1);
        bb2 = BB_from_string(str2);
        nbb2 = BB_not(bb2);

        assert(BB_eq(bb1, nbb2));
        assert(BB_eq(nbb1, bb2));

        free(bb1);
        free(nbb1);
        free(bb2);
        free(nbb2);
    }

    fclose(fp);
}

void BB_test_and() {
    int num_tests;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN], str3[STR_MAX_LEN];
    BigBool *bb1, *bb2, *bb3, *bb12, *bb21;

    FILE *fp = fopen(file_and, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_and);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %s %s", str1, str2, str3);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb3 = BB_from_string(str3);
        bb12 = BB_and(bb1, bb2);
        bb21 = BB_and(bb2, bb1);

        assert(BB_eq(bb12, bb3));
        assert(BB_eq(bb21, bb3));

        free(bb1);
        free(bb2);
        free(bb3);
        free(bb12);
        free(bb21);
    }

    fclose(fp);
}

void BB_test_or() {
    int num_tests;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN], str3[STR_MAX_LEN];
    BigBool *bb1, *bb2, *bb3, *bb12, *bb21;

    FILE *fp = fopen(file_or, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_or);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %s %s", str1, str2, str3);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb3 = BB_from_string(str3);
        bb12 = BB_or(bb1, bb2);
        bb21 = BB_or(bb2, bb1);

        assert(BB_eq(bb12, bb3));
        assert(BB_eq(bb21, bb3));

        free(bb1);
        free(bb2);
        free(bb3);
        free(bb12);
        free(bb21);
    }

    fclose(fp);
}

void BB_test_xor() {
    int num_tests;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN], str3[STR_MAX_LEN];
    BigBool *bb1, *bb2, *bb3, *bb12, *bb21;

    FILE *fp = fopen(file_xor, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_xor);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %s %s", str1, str2, str3);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb3 = BB_from_string(str3);
        bb12 = BB_xor(bb1, bb2);
        bb21 = BB_xor(bb2, bb1);

        assert(BB_eq(bb12, bb3));
        assert(BB_eq(bb21, bb3));

        free(bb1);
        free(bb2);
        free(bb3);
        free(bb12);
        free(bb21);
    }

    fclose(fp);
}

/********** SHIFTS **********/

void BB_test_shift_left() {
    int num_tests, n;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
    BigBool *bb1, *bb1s, *bb2;

    FILE *fp = fopen(file_shift_left, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_shift_left);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %d %s", str1, &n, str2);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb1s = BB_shift_left(bb1, n);

        assert(BB_eq(bb1s, bb2));

        free(bb1);
        free(bb2);
        free(bb1s);
    }

    fclose(fp);
}

void BB_test_shift_right() {
    int num_tests, n;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
    BigBool *bb1, *bb1s, *bb2;

    FILE *fp = fopen(file_shift_right, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_shift_right);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %d %s", str1, &n, str2);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb1s = BB_shift_right(bb1, n);

        assert(BB_eq(bb1s, bb2));

        free(bb1);
        free(bb2);
        free(bb1s);
    }

    fclose(fp);
}

void BB_test_cycle_shift_left() {
    int num_tests, n;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
    BigBool *bb1, *bb1s, *bb2;

    FILE *fp = fopen(file_cycle_shift_left, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_cycle_shift_left);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %d %s", str1, &n, str2);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb1s = BB_cycle_shift_left(bb1, n);

        assert(BB_eq(bb1s, bb2));

        free(bb1);
        free(bb2);
        free(bb1s);
    }

    fclose(fp);
}

void BB_test_cycle_shift_right() {
    int num_tests, n;
    char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
    BigBool *bb1, *bb1s, *bb2;

    FILE *fp = fopen(file_cycle_shift_right, "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл %s\n", file_cycle_shift_right);
        return;
    }

    fscanf(fp, "%d", &num_tests);
    for (int i = 1; i <= num_tests; ++i) {
        fscanf(fp,"%s %d %s", str1, &n, str2);
        bb1 = BB_from_string(str1);
        bb2 = BB_from_string(str2);
        bb1s = BB_cycle_shift_right(bb1, n);

        assert(BB_eq(bb1s, bb2));

        free(bb1);
        free(bb2);
        free(bb1s);
    }

    fclose(fp);
}

/********** RANDOM **********/

void BB_test_random_eq_self(int n) {
    BigBool *bb;

    for (int i = 0; i < n; ++i) {
        bb = BB_random(rand() % VEC_MAX_LEN + 1);
        assert(BB_eq(bb, bb));
        free(bb);
    }
}

void BB_test_random_and(int n) {
    BigBool *zero, *one, *bb, *bb0, *bb1;

    zero = BB_ones(0);
    one = BB_ones(VEC_MAX_LEN);

    for (int i = 0; i < n; ++i) {
        bb = BB_random(rand() % VEC_MAX_LEN + 1);
        bb0 = BB_and(bb, zero);
        bb1 = BB_and(bb, one);

        assert(BB_eq(bb0, zero));
        assert(BB_eq(bb1, bb));

        free(bb);
        free(bb0);
        free(bb1);
    }

    free(zero);
    free(one);
}

void BB_test_random_or(int n) {
    BigBool *zero, *one, *bb, *bb0, *bb1;

    zero = BB_ones(0);
    one = BB_ones(VEC_MAX_LEN);

    for (int i = 0; i < n; ++i) {
        bb = BB_random(rand() % VEC_MAX_LEN + 1);
        bb0 = BB_or(bb, zero);
        bb1 = BB_or(bb, one);

        assert(BB_eq(bb0, bb));
        assert(BB_eq(bb1, one));

        free(bb);
        free(bb0);
        free(bb1);
    }

    free(zero);
    free(one);
}

void BB_test_random_xor(int n) {
    BigBool *zero, *one, *bb, *bb0, *bb1, *bbn, *bbx;

    zero = BB_ones(0);
    one = BB_ones(VEC_MAX_LEN);

    for (int i = 0; i < n; ++i) {
        bb = BB_random(rand() % VEC_MAX_LEN + 1);
        bb0 = BB_xor(bb, zero);
        bb1 = BB_xor(bb, one);
        bbx = BB_xor(bb, bb);
        bbn = BB_not(bb);

        assert(BB_eq(bb0, bb));
        assert(BB_eq(bb1, bbn));
        assert(BB_eq(bbx, zero));

        free(bb);
        free(bb0);
        free(bb1);
        free(bbn);
        free(bbx);
    }

    free(zero);
    free(one);
}

void BB_test_random_xor_from_and_or(int n) {
    BigBool *bb1, *bb2, *bb3, *l, *r;
    // l: x ^ y = (x | y) & not(x & y) :r

    for (int i = 0; i < n; ++i) {
        bb1 = BB_random(rand() % VEC_MAX_LEN + 1);
        bb2 = BB_random(rand() % VEC_MAX_LEN + 1);

        bb3 = BB_assign_not(BB_and(bb1, bb2));
        l = BB_xor(bb1, bb2);
        r = BB_assign_and(BB_or(bb1, bb2), bb3);

        assert(BB_eq(l, r));

        free(bb1);
        free(bb2);
        free(bb3);
        free(l);
        free(r);
    }
}

void BB_test_de_morgan(int n) {
    BigBool *bb1, *bb2, *nbb1, *nbb2;
    BigBool *l1, *r1, *l2, *r2;
    // l1: not(x & y) == not(x) | not(y) :r1
    // l2: not(x | y) == not(x) & not(y) :r2

    for (int i = 0; i < n; ++i) {
        bb1 = BB_random(rand() % VEC_MAX_LEN + 1);
        bb2 = BB_random(rand() % VEC_MAX_LEN + 1);
        nbb1 = BB_not(bb1);
        nbb2 = BB_not(bb2);

        l1 = BB_assign_not(BB_and(bb1, bb2));
        r1 = BB_or(nbb1, nbb2);
        l2 = BB_assign_not(BB_or(bb1, bb2));
        r2 = BB_and(nbb1, nbb2);

        assert(BB_eq(l1, r1));
        assert(BB_eq(l2, r2));

        free(bb1);
        free(bb2);
        free(nbb1);
        free(nbb2);
        free(l1);
        free(r1);
        free(l2);
        free(r2);
    }
}