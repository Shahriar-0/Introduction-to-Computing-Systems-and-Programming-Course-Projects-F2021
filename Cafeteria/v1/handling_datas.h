#pragma once

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    char* username;
    int id;
    char* password;
} User;

typedef struct Hall {
    char* name;
    int id;
    int capacity;
} Hall;

typedef struct Food {
    char* name;
    int id;
    int price;
    int capacity;
    Hall hall;
    int day;
} Food;

typedef struct Coupon {
    int id;
    int percantage;
    int capacity;
} Coupon;

void load_users(User**, int*);
void load_foods(Food**, int*);
void load_halls(Hall**, int*);
void load_coupons(Coupon**, int*);
void read_word_from_file(FILE*, char**, int*);
void checking_for_null_pointer(void*);
void freeing(User*, int, Food*, int, Hall*, int, Coupon*, int);