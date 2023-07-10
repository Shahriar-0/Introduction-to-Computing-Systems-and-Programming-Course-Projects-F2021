#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum account_status { LOGGED_IN,
                      LOGGED_OUT };
enum program_mode { TERMINATE,
                    CONTINUE };
enum mode { HALL,
            FOOD,
            USER,
            COUPON,
            RESERVED_FOOD };

enum searching_modes_in_users { USERNAME,
                                ID };

#define EXIT   		     "exit"
#define SIGNUP 		     "signup"
#define LOGIN            "login"
#define ADMIN_PASS       "admin"
#define ADMIN_USERNAME   "admin"
#define FOOD_REPORT      "food_report"
#define HALL_REPORT      "hall_report"
#define GET_MONEY        "get_money"
#define SHOW_MONEY       "show_money"
#define MENU             "menu"
#define RESERVE 	     "reserve"
#define RESERVE_DISCOUNT "reserve_discount"
#define SHOW_RESERVED    "show_reserved"
#define LOGOUT           "logout"
#define ADD_HALL 		 "add_hall"
#define ADD_FOOD 		 "add_food"
#define ADD_COUPON 	  	 "add_coupon"

#define NUMBER_OF_DAYS 7

#define USERS_PATH "users.txt"
#define FOOD_PATH  "food_report.txt"
#define HALL_PATH  "hall_report.txt"


typedef struct Reserved_food {  
    char* food_name;
    int price;
    int food_id;
    char* hall_name;
    struct Reserved_food* next;
} Reserved_food;

typedef struct User { 
    char* username;
    char* password;
    int id;
    int money;
    Reserved_food* days[NUMBER_OF_DAYS];
    struct User* next;
} User;

typedef struct Hall { 
    char* name;
    int id;
    int capacity;
    int reserved[NUMBER_OF_DAYS];
    int profit[NUMBER_OF_DAYS];
    struct Hall* next;
} Hall;

typedef struct Food { 
    char* name;
    int id;
    int price;
    Hall* hall; 
    int capacity;
    int reserved;
    int profit;
    struct Food* next;
} Food;

typedef struct Coupon {  
    int id;
    int percantage;
    int capacity;
    struct Coupon* next;
} Coupon;