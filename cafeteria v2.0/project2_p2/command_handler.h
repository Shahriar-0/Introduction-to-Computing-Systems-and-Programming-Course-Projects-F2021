#pragma once
//this library will handle the commands that we get from user and admin
#include "file_handler.h"
int entering_page(bool*, User**, bool*, User*);
void signing_up(User*);
void logging_in(bool*, User**, bool*, User*);
void empty_stdin();
User* search_in_users(User*, char*, int);
Hall* search_in_halls(Hall*, int);
Coupon* search_in_coupons(Coupon*, int);
Food* search_in_foods(Food* array_head[NUMBER_OF_DAYS], int, int*);
int getting_command_from_user(bool*, User**, Food* array_head[NUMBER_OF_DAYS], Coupon*);
void show_menu(Food* array_head[NUMBER_OF_DAYS]);
void reserve(User*, Food* array_of_heads_of_foods[NUMBER_OF_DAYS]);
void reserve_discount(User*, Food* array_of_heads_of_foods[NUMBER_OF_DAYS], Coupon*);
void show_reserved(User*);
int getting_command_from_admin(bool*, Food* array_head[NUMBER_OF_DAYS], Coupon*, Hall*);
void add_coupon(Coupon*);
void add_hall(Hall*);
void add_food(Food* array_head[NUMBER_OF_DAYS], Hall*);
void hall_report(Hall*);
void food_report(Food* array_head[NUMBER_OF_DAYS]);