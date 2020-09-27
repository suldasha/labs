#ifndef BIGBOOL_TESTS_H
#define BIGBOOL_TESTS_H

void BB_test_all(int);
void BB_test_not_random();
void BB_test_random(int);

void BB_test_str_bb_str();
void BB_test_uint64_bb_uint64();
void BB_test_ones();

void BB_test_eq();
void BB_test_copy();

void BB_test_not();
void BB_test_and();
void BB_test_or();
void BB_test_xor();

void BB_test_shift_left();
void BB_test_shift_right();
void BB_test_cycle_shift_left();
void BB_test_cycle_shift_right();

void BB_test_random_eq_self(int);
void BB_test_random_and(int);
void BB_test_random_or(int);
void BB_test_random_xor(int);
void BB_test_random_xor_from_and_or(int);
void BB_test_de_morgan(int);

#endif //BIGBOOL_TESTS_H
