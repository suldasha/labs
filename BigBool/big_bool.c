#include "big_bool.h"
#include <string.h>
#include <stdlib.h>

void BB_fix_last(BigBool *bb) {     // Пересчёт last_bit и last_part
    bb->last_bit = 0;
    bb->last_part = 0;

    for (int i = MAX_PARTS - 1; i >= 0; --i)
        if (bb->parts[i]) {
            bb->last_part = i;

            for (int j = 7; j >= 0; --j)
                if (bb->parts[i] & (1 << j)) {
                    bb->last_bit = i * 8 + j;
                    break;
                }

            break;
        }
}

/********** CREATE ***********/

BigBool* BB_from_string(char *str) {
    size_t len = strlen(str);

    if (len > 8 * MAX_PARTS)
        return len = 8 * MAX_PARTS;

    BigBool *bb = malloc(sizeof(BigBool));
    bb->last_part = 0;
    bb->last_bit = 0;
    memset(bb->parts, 0, MAX_PARTS);

    for (int i = 0; i < len; ++i)
        if (str[len - i - 1] == '1') {
            bb->parts[i / 8] |= (1 << (i % 8));
            bb->last_part = i / 8;
            bb->last_bit = i;
        }

    return bb;
}

BigBool* BB_from_uint64(uint64_t x) {
    BigBool *bb = malloc(sizeof(BigBool));
    memcpy(bb->parts, &x, sizeof(x));
    memset(bb->parts + sizeof(x), 0, MAX_PARTS - sizeof(x));
    BB_fix_last(bb);

    return bb;
}

BigBool* BB_copy(BigBool *bb) {
    BigBool *res = malloc(sizeof(BigBool));
    memcpy(res, bb, sizeof(BigBool));
    return res;
}

BigBool* BB_random(int max_len) {
    if (max_len > 8 * MAX_PARTS)
        max_len = 8 * MAX_PARTS;

    BigBool *bb = malloc(sizeof(BigBool));
    memset(bb->parts, 0, MAX_PARTS);

    for (int i = 0; i <= (max_len - 1) / 8; ++i)
        bb->parts[i] = rand();

    if (max_len % 8)
        bb->parts[(max_len - 1) / 8] &= (1 << (max_len % 8)) - 1;

    BB_fix_last(bb);
    return bb;
}

BigBool* BB_ones(size_t n) {
    if (n > 8 * MAX_PARTS)
        n = 8 * MAX_PARTS;

    BigBool *bb = malloc(sizeof(BigBool));

    if (n) {
        bb->last_part = (n - 1) / 8;
        bb->last_bit = n - 1;
    } else {
        bb->last_part = 0;
        bb->last_bit = 0;
    }

    if (n % 8) {
        memset(bb->parts, -1, n / 8);
        bb->parts[n / 8] = (1 << (n % 8)) - 1;
        memset(bb->parts + n / 8 + 1, 0, MAX_PARTS - n / 8 - 1);
    } else {
        memset(bb->parts, -1, n / 8);
        memset(bb->parts + n / 8, 0, MAX_PARTS - n / 8);
    }

    return bb;
}

char* BB_to_string(BigBool *bb) {
    char *str = malloc(bb->last_bit + 2);
    for (int i = 0; i < bb->last_bit + 1; ++i)
        if (bb->parts[i / 8] & (1 << (i % 8)))
            str[bb->last_bit - i] = '1';
        else
            str[bb->last_bit - i] = '0';
    str[bb->last_bit + 1] = '\0';

    return str;
}

bool BB_eq(BigBool *bb1, BigBool *bb2) {
    return bb1->last_bit == bb2->last_bit
        && bb1->last_part == bb2->last_part
        && memcmp(bb1->parts, bb2->parts, MAX_PARTS) == 0;
}

/********** NOT, AND, OR, XOR *********/

BigBool* BB_assign_not(BigBool *bb) {
    for (int i = 0; i < MAX_PARTS; ++i)
        bb->parts[i] = ~bb->parts[i];
    BB_fix_last(bb);

    return bb;
}

BigBool* BB_assign_and(BigBool *bb1, BigBool *bb2) {
    for (int i = 0; i < MAX_PARTS; ++i)
        bb1->parts[i] &= bb2->parts[i];
    BB_fix_last(bb1);

    return bb1;
}

BigBool* BB_assign_or(BigBool *bb1, BigBool *bb2) {
    for (int i = 0; i < MAX_PARTS; ++i)
        bb1->parts[i] |= bb2->parts[i];
    BB_fix_last(bb1);

    return bb1;
}

BigBool* BB_assign_xor(BigBool *bb1, BigBool *bb2) {
    for (int i = 0; i < MAX_PARTS; ++i)
        bb1->parts[i] ^= bb2->parts[i];
    BB_fix_last(bb1);

    return bb1;
}

BigBool* BB_not(BigBool *bb) {
    return BB_assign_not(BB_copy(bb));
}

BigBool* BB_and(BigBool *bb1, BigBool *bb2) {
    return BB_assign_and(BB_copy(bb1), bb2);
}

BigBool* BB_or(BigBool *bb1, BigBool *bb2) {
    return BB_assign_or(BB_copy(bb1), bb2);
}

BigBool* BB_xor(BigBool *bb1, BigBool *bb2) {
    return BB_assign_xor(BB_copy(bb1), bb2);
}

/********** SHIFTS **********/

BigBool* BB_assign_shift_left(BigBool *bb, int n) {
    if (n < 0)
        return BB_assign_shift_right(bb, -n);

    int shift_parts = n / 8;
    int shift_bits = n % 8;

    if (shift_parts >= MAX_PARTS) {
        memset(bb->parts, 0, MAX_PARTS);
        bb->last_part = 0;
        bb->last_bit = 0;
        shift_bits = 0;
    } else if (shift_parts) {
        memmove(bb->parts + shift_parts, bb->parts, MAX_PARTS - shift_parts);
        memset(bb->parts, 0, shift_parts);
        BB_fix_last(bb);
    }

    if (shift_bits) {
        for (int i = MAX_PARTS - 1; i > 0; --i) {
            bb->parts[i] = (bb->parts[i] << shift_bits) | (bb->parts[i - 1] >> (8 - shift_bits));
        }
        bb->parts[0] <<= shift_bits;
        BB_fix_last(bb);
    }

    return bb;
}

BigBool* BB_assign_shift_right(BigBool *bb, int n) {
    if (n < 0)
        return BB_assign_shift_left(bb, -n);

    int shift_parts = n / 8;
    int shift_bits = n % 8;

    if (shift_parts >= MAX_PARTS) {
        memset(bb->parts, 0, MAX_PARTS);
        bb->last_part = 0;
        bb->last_bit = 0;
        shift_bits = 0;
    } else if (shift_parts) {
        memmove(bb->parts, bb->parts + shift_parts, MAX_PARTS - shift_parts);
        memset(bb->parts + MAX_PARTS - shift_parts, 0, shift_parts);
        BB_fix_last(bb);
    }

    if (shift_bits) {
        for (int i = 0; i < MAX_PARTS - 1; ++i) {
            bb->parts[i] = (bb->parts[i] >> shift_bits) | (bb->parts[i + 1] << (8 - shift_bits));
        }
        bb->parts[MAX_PARTS - 1] >>= shift_bits;
        BB_fix_last(bb);
    }

    return bb;
}

BigBool* BB_assign_cycle_shift_left(BigBool *bb, int n) {
    n = n % (8 * MAX_PARTS);

    if (n < 0)
        n += 8 * MAX_PARTS;

    BigBool* tmp = BB_shift_left(bb, n);
    BB_assign_shift_right(bb, 8 * MAX_PARTS - n);
    BB_assign_or(bb, tmp);
    free(tmp);

    return bb;
}

BigBool* BB_assign_cycle_shift_right(BigBool *bb, int n) {
    return BB_assign_cycle_shift_left(bb, -n);
}

BigBool* BB_shift_left(BigBool *bb, int n) {
    return BB_assign_shift_left(BB_copy(bb), n);
}

BigBool* BB_shift_right(BigBool *bb, int n) {
    return BB_assign_shift_right(BB_copy(bb), n);
}

BigBool* BB_cycle_shift_left(BigBool *bb, int n) {
    return BB_assign_cycle_shift_left(BB_copy(bb), n);
}

BigBool* BB_cycle_shift_right(BigBool *bb, int n) {
    return BB_assign_cycle_shift_right(BB_copy(bb), n);
}
