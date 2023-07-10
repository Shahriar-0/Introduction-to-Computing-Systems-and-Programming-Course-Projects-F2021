#pragma once
#include "handling_datas.h"

enum program_mode { CONTINUE,
                    TERMINATE };
enum account_status { LOG_OUT,
                      LOG_IN };
enum searching { NOT_FOUND,
                 FOUND };

int enetring_app(User**, int*, bool*, bool*);
int signup_user(User**, int);
void read_word_from_user(char**);
void logging_in(User*, int, bool*, bool*);
int searching_in_users(User*, int, char*, int, int*);
int searching_in_halls(Hall*, int, int);
int searching_in_foods(Food*, int, int);
int searching_in_coupons(Coupon*, int, int);
int get_order_from_user(bool*);
int get_order_from_admin(Food**, int*, Hall**, int*, Coupon**, int*, bool*);
int add_hall(Hall**, int);
int add_coupon(Coupon**, int);
int add_food(Food**, int, Hall*, int);