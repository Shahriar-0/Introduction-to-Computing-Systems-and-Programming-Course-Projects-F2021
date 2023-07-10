#define _CRT_SECURE_NO_WARNINGS

#include "user_handler.h"

void read_word_from_user(char** word) {
    *word = NULL;
    int word_size = 0;
    char letter = 'a';
    while (letter != ' ' && letter != '\n') {
        scanf("%c", &letter);
        word_size++;
        *word = (char*)realloc(*word, word_size);
        checking_for_null_pointer(*word);
        *(*word + word_size - 1) = letter;
    }
    *(*word + word_size - 1) = NULL;  // just to make it like other strings
}

int searching_in_users(User* list_of_users, int size, char* name, int identity, int* pointer_to_index) {
    for (int i = 0; i < size; i++) {
        if (!strcmp((list_of_users + i)->username, name)) {
            if (identity != -1)
                printf("an account with that username exists\n***************************************\n");
            *pointer_to_index = i;
            return FOUND;
        } else if ((list_of_users + i)->id == identity) {
            printf("an account with that id exists\n***************************************\n");
            return FOUND;
        }
    }
    return NOT_FOUND;
}

int signup_user(User** pointer_to_list_of_users, int size) {
    char *name, *pass;
    int identity, index;
    read_word_from_user(&name);
    scanf("%d ", &identity);
    read_word_from_user(&pass);

    if (searching_in_users(*pointer_to_list_of_users, size, name, identity, &index))
        return 0;

    *pointer_to_list_of_users = (User*)realloc(*pointer_to_list_of_users, (size + 1) * sizeof(User));
    checking_for_null_pointer(*pointer_to_list_of_users);
    (*pointer_to_list_of_users + size)->username = name;
    (*pointer_to_list_of_users + size)->password = pass;
    (*pointer_to_list_of_users + size)->id = identity;
    printf("signed up successfully\n***************************************\n");
    return 1;
}

void logging_in(User* pointer_to_list_of_users, int size,
                bool* pointer_to_user, bool* pointer_to_admin) {
    char *username, *password;
    read_word_from_user(&username);
    read_word_from_user(&password);

    if (!strcmp(username, "admin") && !strcmp(password, "admin")) {
        printf("welcome admin\n");
        *pointer_to_admin = LOG_IN;
        free(username);
        free(password);
        return;
    }

    int index;
    if (searching_in_users(pointer_to_list_of_users, size, username, -1, &index)) {
        if (!strcmp((pointer_to_list_of_users + index)->password, password)) {
            printf("welcom %s :)\n", username);
            *pointer_to_user = LOG_IN;
        } else
            printf("check your password again\n***************************************\n");
        free(username);
        free(password);
        return;
    }

    printf("sorry, we didn't find you in our users :(\nplease check your spelling or signup first\n***************************************\n");
    free(username);
    free(password);
    return;
}

int enetring_app(User** pointer_to_list_of_users, int* size,
                 bool* pointer_to_user, bool* pointer_to_admin) {
    char* order = NULL;
    read_word_from_user(&order);

    if (!strcmp(order, "terminate"))
        return TERMINATE;

    if (!strcmp(order, "signup"))
        *size += signup_user(pointer_to_list_of_users, *size);
    else if (!strcmp(order, "login"))
        logging_in(*pointer_to_list_of_users, *size, pointer_to_user, pointer_to_admin);
    else if (strcmp(order, ""))
        printf("acces denied\n***************************************\n");
    free(order);
    return CONTINUE;
}

int get_order_from_user(bool* pointer_to_user) {
    char* order;
    read_word_from_user(&order);
    if (!strcmp(order, "terminate")) {
        free(order);
        return TERMINATE;
    }

    if (!strcmp(order, "logout")) {
        *pointer_to_user = LOG_OUT;
        printf("logged out successfully\n***************************************\n");
    }

    else if (!strcmp(order, "add_hall") || !strcmp(order, "add_food") || !strcmp(order, "add_coupon"))
        printf("permission denied\n***************************************\n");

    else if (!strcmp(order, "login"))
        printf("you are currently logged in\nif you want to login with another account,\nplease logout first\n***************************************\n");

    else if (strcmp(order, ""))
        printf("not valid\n***************************************\n");

    free(order);
    return CONTINUE;
}

int get_order_from_admin(Food** list_of_foods, int* foods_size,
                         Hall** list_of_halls, int* halls_size,
                         Coupon** list_of_coupons, int* coupons_size,
                         bool* pointer_to_admin) {
    char* order;
    read_word_from_user(&order);
    if (!strcmp(order, "terminate")) {
        free(order);
        return TERMINATE;
    }

    if (!strcmp(order, "add_hall"))
        *halls_size += add_hall(list_of_halls, *halls_size);

    else if (!strcmp(order, "add_food"))
        *foods_size += add_food(list_of_foods, *foods_size, *list_of_halls, *halls_size);

    else if (!strcmp(order, "add_coupon"))
        *coupons_size += add_coupon(list_of_coupons, *coupons_size);

    else if (!strcmp(order, "logout")) {
        *pointer_to_admin = LOG_OUT;
        printf("logged out successfully\n***************************************\n");
    }

    else if (strcmp(order, ""))
        printf("not valid\n***************************************\n");

    free(order);
    return CONTINUE;
}

int searching_in_halls(Hall* list_of_halls, int size, int identity) {
    for (int i = 0; i < size; i++) {
        if ((list_of_halls + i)->id == identity) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}

int add_hall(Hall** list_of_halls, int size) {
    char* name;
    read_word_from_user(&name);
    int hall_id, capacity;
    scanf("%d %d", &hall_id, &capacity);

    if (searching_in_halls(*list_of_halls, size, hall_id)) {
        printf("that id for hall is not unqiue\n***************************************\n");
        return 0;  // not unique
    }

    *list_of_halls = (Hall*)realloc(*list_of_halls, (size + 1) * sizeof(Hall));
    checking_for_null_pointer(*list_of_halls);
    (*list_of_halls + size)->name = name;
    (*list_of_halls + size)->id = hall_id;
    (*list_of_halls + size)->capacity = capacity;
    printf("added successfully\n***************************************\n");
    return 1;
}

int add_coupon(Coupon** list_of_coupons, int size) {
    int identity, percentage, capacity;
    scanf("%d %d %d", &identity, &percentage, &capacity);

    if (searching_in_coupons(*list_of_coupons, size, identity)) {
        printf("that id for coupon is not unqiue\n***************************************\n");
        return 0;
    }

    *list_of_coupons = (Coupon*)realloc(*list_of_coupons, (size + 1) * sizeof(Coupon));
    checking_for_null_pointer(*list_of_coupons);
    (*list_of_coupons + size)->id = identity;
    (*list_of_coupons + size)->percantage = percentage;
    (*list_of_coupons + size)->capacity = capacity;
    printf("added successfully\n***************************************\n");
    return 1;
}

int add_food(Food** list_of_foods, int foods_size, Hall* list_of_halls, int halls_size) {
    char* name;
    int food_id, price, capacity, hall_id, day;
    read_word_from_user(&name);
    scanf("%d %d %d %d %d", &food_id, &price, &capacity, &hall_id, &day);

    if (day > 7 || day < 1) {
        printf("day is not valid\n***************************************\n");
        return 0;
    }

    if (searching_in_halls(list_of_halls, halls_size, hall_id)) {
        if (searching_in_foods(*list_of_foods, foods_size, food_id)) {
            printf("sorry that food id exists\n***************************************\n");
            return 0;
        }
        *list_of_foods = (Food*)realloc(*list_of_foods, (foods_size + 1) * sizeof(Food));
        (*list_of_foods + foods_size)->name = name;
        (*list_of_foods + foods_size)->id = food_id;
        (*list_of_foods + foods_size)->hall.id = hall_id;
        (*list_of_foods + foods_size)->capacity = capacity;
        (*list_of_foods + foods_size)->price = price;
        (*list_of_foods + foods_size)->day = day;
        printf("added successfully\n***************************************\n");
        return 1;
    }

    else {
        printf("sorry that hall id doesn't exists\n***************************************\n");
        return 0;
    }
}

int searching_in_foods(Food* list_of_foods, int size, int identity) {
    for (int i = 0; i < size; i++) {
        if ((list_of_foods + i)->id == identity) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}

int searching_in_coupons(Coupon* list_of_coupons, int size, int identity) {
    for (int i = 0; i < size; i++) {
        if ((list_of_coupons + i)->id == identity) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}