#pragma once
#include "requisities.h"
void* create_dummy(int);
void is_null_pointer(void*);
void load_data(User*);
char* read_word_from_file(FILE*);
char* read_word_from_console();
void freeing(User*, Coupon* , Food* array_of_head_of_foods[NUMBER_OF_DAYS], Hall*);