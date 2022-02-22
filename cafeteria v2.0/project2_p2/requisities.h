#pragma once
#define _CRT_SECURE_NO_WARNINGS
//this header is to keep our includes and structures that we need throughout the program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//enums that we need to make our code more readable
enum account_status{LOGGED_IN, LOGGED_OUT};		
enum program_mode {TERMINATE, CONTINUE};
enum mode{HALL, FOOD, USER, COUPON, RESERVED_FOOD};
enum searching_modes_in_users {USERNAME, ID};
//define that we need for checking command
#define EXIT "exit"
#define SIGNUP "signup"
#define LOGIN "login"
#define ADMIN_PASS "admin"
#define ADMIN_USERNAME "admin"
#define FOOD_REPORT "food_report"
#define HALL_REPORT "hall_report"
#define GET_MONEY "get_money"
#define SHOW_MONEY "show_money"
#define MENU "menu"
#define RESERVE "reserve"
#define DRESERVE_DISCOUNT "reserve_discount"
#define SHOW_RESERVED "show_reserved"
#define LOGOUT "logout"
#define ADD_HALL "add_hall"
#define ADD_FOOD "add_food"
#define ADD_COUPON "add_coupon"
#define NUMBER_OF_DAYS 7
//define that we need for files
#define USERS_PATH "users.txt"
#define FOOD_PATH "food_report.txt"
#define HALL_PATH "hall_report.txt"
//structures that are used
typedef struct Reserved_food {	//the structure for foods that are reserved by our user
	char* food_name;
	int price;
	int food_id;
	char* hall_name;
	struct Reserved_food* next;
}Reserved_food;

typedef struct User {	//the structure for our user
	char* username;
	char* password;
	int id;
	int money;
	Reserved_food* days[NUMBER_OF_DAYS];	//cause we have 7 days that each are linked list
	struct User* next;
}User;

typedef struct Hall {	//our hall's structure
	char* name;
	int id;
	int capacity;
	int reserved[NUMBER_OF_DAYS];	//to store datas for 7 different days
	int profit[NUMBER_OF_DAYS];
	struct Hall* next;
}Hall;

typedef struct Food {	//the structure for handling our foods
	char* name;
	int id;
	int price;
	Hall* hall;	//to connect it with corresponding hall
	int capacity;
	int reserved;
	int profit;
	struct Food* next;
}Food;

typedef struct Coupon {	//our coupon structue
	int id;
	int percantage;
	int capacity;
	struct Coupon* next;
}Coupon;