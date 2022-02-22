#pragma once
//this header file contains funtions that help us load data from our drive
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdbool.h>

typedef struct User { //this is our user's structure
	char* username;
	int id;
	char* password;
}User;

typedef struct Hall { //this is our hall's structure
	char* name;
	int id;
	int capacity;
}Hall;

typedef struct Food{ //this is our food's structure
	char* name;
	int id;
	int price;
	int capacity;
	Hall hall;
	int day;
}Food;

typedef struct Coupon {//this is our coupon's structure
	int id;
	int percantage;
	int capacity;
}Coupon;

void load_users(User**, int*);
void load_foods(Food**, int*);
void load_halls(Hall**, int*);
void load_coupons(Coupon**, int*);
void read_word_from_file(FILE*, char**, int*);
void checking_for_null_pointer(void*);
void freeing(User*, int, Food*, int, Hall*, int, Coupon*, int);