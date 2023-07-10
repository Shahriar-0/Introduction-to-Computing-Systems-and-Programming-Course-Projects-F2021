#include "command_handler.h"

int entering_page(bool* pointer_to_user_status, User** pointer_to_user, bool* pointer_to_admin_status, User* head_of_users) {
    char* command = read_word_from_console();
    if (!strcmp(command, EXIT)) {
        free(command);
        return TERMINATE;
    }

    if (!strcmp(command, LOGIN))
        logging_in(pointer_to_user_status, pointer_to_user,
                   pointer_to_admin_status, head_of_users);
    else if (!strcmp(command, SIGNUP))
        signing_up(head_of_users);
    else
        printf("acces denied\n*********************************************\n");

    empty_stdin();
    free(command);
    return CONTINUE;
}

void empty_stdin() {
    fseek(stdin, 0, SEEK_END);
    int num = ftell(stdin);
    if (num) {
        char c;
        while ((c = getc(stdin)) != '\n' && c != EOF) {
        }
    }
}

void logging_in(bool* pointer_to_user_status, User** pointer_to_user,
                bool* pointer_to_admin_status, User* head_of_users) {
    char* username = read_word_from_console();
    char* password = read_word_from_console();
    if (!strcmp(username, ADMIN_USERNAME) && !strcmp(password, ADMIN_PASS)) {
        *pointer_to_admin_status = LOGGED_IN;
        printf("welcome admin\n");
    } else {
        (*pointer_to_user) = search_in_users(head_of_users, username, USERNAME);

        if ((*pointer_to_user) == NULL) {
            printf("sorry, we didn't find you in our users:(\n");
            printf("check your spelling or signup\n");
            printf("*********************************************\n");
        } else {
            if (!strcmp((*pointer_to_user)->password, password)) {
                printf("welcome %s :)\n", (*pointer_to_user)->username);
                *pointer_to_user_status = LOGGED_IN;
            } else {
                (*pointer_to_user) = NULL;
                printf("please check your password again\n");
                printf("*********************************************\n");
            }
        }
    }
    free(username);
    free(password);
}

void signing_up(User* head) {
    char* username = read_word_from_console();
    char* id_string = read_word_from_console();
    char* password = read_word_from_console();
    User* ptr = search_in_users(head, username, USERNAME);

    if (ptr != NULL) {  // username is not unique
        printf("an account with that username already exist\n*********************************************\n");
        free(username);
        free(password);
    }

    else {
        ptr = search_in_users(head, id_string, ID);
        if (ptr != NULL) {  // id is not unique
            printf("an account with that id already exist\n*********************************************\n");
            free(username);
            free(password);
        } else {
            // everything is unique and we have to add it
            User* temp = (User*)malloc(sizeof(User));

            temp->next = head->next;
            head->next = temp;

            temp->username = username;
            temp->id = atoi(id_string);
            temp->password = password;
            temp->money = 0;
            for (int i = 0; i < NUMBER_OF_DAYS; i++)
                temp->days[i] = (Reserved_food*)create_dummy(RESERVED_FOOD);
            printf("added successfully\n*********************************************\n");
        }
    }
    free(id_string);
}

User* search_in_users(User* head, char* word, int mode) {
    head = head->next;
    if (mode == USERNAME) {
        while (head) {
            if (!strcmp(head->username, word))
                return head;
            head = head->next;
        }
    } else if (mode == ID) {
        int id = atoi(word);
        while (head) {
            if (head->id == id)
                return head;
            head = head->next;
        }
    }
    return NULL;
}

int getting_command_from_user(bool* pointer_to_user_status,
                              User** pointer_to_user,
                              Food* array_head_of_food[NUMBER_OF_DAYS],
                              Coupon* head_of_coupons) {
    char* command = read_word_from_console();
    if (!strcmp(command, EXIT)) {
        free(command);
        return TERMINATE;
    }

    if (!strcmp(command, LOGOUT)) {
        *pointer_to_user_status = LOGGED_OUT;
        (*pointer_to_user) = NULL;
        printf("logged out successfully\n*********************************************\n");
    }

    else if (!strcmp(command, ADD_FOOD) || !strcmp(command, ADD_COUPON) ||
             !strcmp(command, ADD_HALL) || !strcmp(command, FOOD_REPORT) ||
             !strcmp(command, HALL_REPORT))
        printf("permission denied\n*********************************************\n");

    else if (!strcmp(command, GET_MONEY)) {
        int money;
        scanf("%d", &money);
        (*pointer_to_user)->money += money;
        printf("added\n*********************************************\n");
    }

    else if (!strcmp(command, SHOW_MONEY))
        printf("%d\n*********************************************\n", (*pointer_to_user)->money);
    else if (!strcmp(command, MENU))
        show_menu(array_head_of_food);
    else if (!strcmp(command, RESERVE))
        reserve(*pointer_to_user, array_head_of_food);
    else if (!strcmp(command, RESERVE_DISCOUNT))
        reserve_discount(*pointer_to_user, array_head_of_food, head_of_coupons);
    else if (!strcmp(command, SHOW_RESERVED))
        show_reserved(*pointer_to_user);
    else
        printf("not valid\n*********************************************\n");

    empty_stdin();
    free(command);
    return CONTINUE;
}

void show_menu(Food* array_head[NUMBER_OF_DAYS]) {
    for (int i = 0; i < NUMBER_OF_DAYS; i++) {
        Food* head = array_head[i]->next;
        printf("%d:\n", i + 1);
        while (head) {
            printf("%s %d %d %s %d\n", head->name, head->price, head->id, head->hall->name, (head->capacity - head->reserved));
            head = head->next;
        }
    }
    printf("*********************************************\n");
}

void reserve(User* pointer_to_user, Food* array_of_heads_of_foods[NUMBER_OF_DAYS]) {
    int food_id, day;
    scanf("%d", &food_id);
    Food* food_ptr = search_in_foods(array_of_heads_of_foods, food_id, &day);
    if (food_ptr == NULL) {
        printf("that food id doesn't exit, please check again\n");
        printf("*********************************************\n");
    }

    else {
        if (food_ptr->capacity == food_ptr->reserved) {
            printf("sorry, we have run out that food\n");
            printf("*********************************************\n");
        }

        else if (food_ptr->hall->capacity == food_ptr->hall->reserved[day]) {
            printf("sorry, the hall is full\n");
            printf("*********************************************\n");
        }

        else if (pointer_to_user->money < food_ptr->price) {
            printf("sorry, you don't have enough money\n");
            printf("*********************************************\n");
        }

        else {
            Reserved_food* temp = (Reserved_food*)malloc(sizeof(Reserved_food));
            temp->next = pointer_to_user->days[day]->next;
            pointer_to_user->days[day]->next = temp;
            temp->food_name = food_ptr->name;
            temp->hall_name = food_ptr->hall->name;
            temp->price = food_ptr->price;
            temp->food_id = food_id;
            pointer_to_user->money -= food_ptr->price;
            food_ptr->hall->reserved[day]++;
            (food_ptr->reserved)++;
            food_ptr->profit += food_ptr->price;
            food_ptr->hall->profit[day] += food_ptr->price;
            printf("reserved successfully\n*********************************************\n");
        }
    }
}

void reserve_discount(User* pointer_to_user,
                      Food* array_of_heads_of_foods[NUMBER_OF_DAYS],
                      Coupon* head_of_coupons) {
    int food_id, coupon_id, day;
    scanf("%d %d", &food_id, &coupon_id);
    Food* food_ptr = search_in_foods(array_of_heads_of_foods, food_id, &day);
    if (food_ptr == NULL) {
        printf("that food id doesn't exit, please check again\n");
        printf("**************************************");
    }

    else {
        Coupon* coupon_ptr = search_in_coupons(head_of_coupons, coupon_id);
        if (coupon_ptr == NULL) {
            printf("that coupon id doesn't exit, please check again\n");
            printf("**************************************");
        }

        else {
            if (food_ptr->capacity == food_ptr->reserved) {
                printf("sorry, we have run out that food\n");
                printf("**************************************");
            }

            else if (food_ptr->hall->capacity == food_ptr->hall->reserved[day]) {
                printf("sorry, the hall is full\n");
                printf("**************************************");
            }

            else if (coupon_ptr->capacity <= 0) {
                printf("sorry, that coupon is no longer valid\n");
                printf("**************************************");
            }

            else if (pointer_to_user->money < (food_ptr->price -
                                               (food_ptr->price * coupon_ptr->percantage / 100))) {
                printf("sorry, you don't have enough money\n");
                printf("**************************************");
            }

            else {
                Reserved_food* temp = (Reserved_food*)malloc(sizeof(Reserved_food));
                temp->next = pointer_to_user->days[day]->next;
                pointer_to_user->days[day]->next = temp;

                temp->food_name = food_ptr->name;
                temp->hall_name = food_ptr->hall->name;
                temp->price = food_ptr->price;
                temp->food_id = food_id;

                pointer_to_user->money -= (food_ptr->price -
                                           (food_ptr->price * coupon_ptr->percantage / 100));
                food_ptr->hall->reserved[day]++;
                (food_ptr->reserved)++;
                food_ptr->profit += (food_ptr->price -
                                     (food_ptr->price * coupon_ptr->percantage / 100));
                food_ptr->hall->profit[day] += (food_ptr->price -
                                                (food_ptr->price * coupon_ptr->percantage / 100));
                (coupon_ptr->capacity)--;
                printf("reserved successfully\n");
                printf("**************************************");
            }
        }
    }
}

void show_reserved(User* pointer_to_user) {
    for (int i = 0; i < NUMBER_OF_DAYS; i++) {
        printf("%d:\n", i + 1);
        Reserved_food* head = pointer_to_user->days[i]->next;
        while (head) {
            printf("%s %d %d %s\n", head->food_name, head->price, head->food_id, head->hall_name);
            head = head->next;
        }
    }
    printf("**************************************");
}

int getting_command_from_admin(bool* pointer_to_admin_status, Food* array_of_head_of_foods[NUMBER_OF_DAYS], Coupon* head_of_coupons, Hall* head_of_halls) {
    char* command = read_word_from_console();
    if (!strcmp(command, EXIT)) {
        free(command);
        return TERMINATE;
    }
    if (!strcmp(command, ADD_COUPON))
        add_coupon(head_of_coupons);
    else if (!strcmp(command, ADD_FOOD))
        add_food(array_of_head_of_foods, head_of_halls);
    else if (!strcmp(command, ADD_HALL))
        add_hall(head_of_halls);
    else if (!strcmp(command, FOOD_REPORT))
        food_report(array_of_head_of_foods);
    else if (!strcmp(command, HALL_REPORT))
        hall_report(head_of_halls);
    else if (!strcmp(command, LOGOUT)) {
        *pointer_to_admin_status = LOGGED_OUT;
        printf("logged out successfully\n");
        printf("**************************************");
    } else {
        printf("not valid\n");
        printf("**************************************");
    }

    empty_stdin();
    free(command);
    return CONTINUE;
}

void add_coupon(Coupon* head) {
    int id, percentage, capacity;
    scanf("%d %d %d", &id, &percentage, &capacity);
    Coupon* ptr = search_in_coupons(head, id);
    if (ptr != NULL) {
        printf("that coupon id is not unique\n");
        printf("**************************************");
    }

    else {
        Coupon* temp = (Coupon*)malloc(sizeof(Coupon));
        temp->next = head->next;
        head->next = temp;

        temp->id = id;
        temp->percantage = percentage;
        temp->capacity = capacity;
        printf("added successfully\n");
        printf("**************************************");
    }
}

Coupon* search_in_coupons(Coupon* head, int id) {
    head = head->next;
    while (head) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return NULL;
}

void add_hall(Hall* head) {
    char* name = read_word_from_console();
    int id, capacity;
    scanf("%d %d", &id, &capacity);
    Hall* ptr = search_in_halls(head, id);

    if (ptr != NULL) {
        printf("that hall id is not unique\n");
        printf("**************************************");
    }

    else {
        Hall* temp = (Hall*)malloc(sizeof(Hall));
        temp->next = head->next;
        head->next = temp;

        temp->name = name;
        temp->id = id;
        temp->capacity = capacity;
        for (int i = 0; i < NUMBER_OF_DAYS; i++) {
            temp->reserved[i] = 0;
            temp->profit[i] = 0;
        }
        printf("added successfully\n");
        printf("**************************************");
    }
}

Hall* search_in_halls(Hall* head, int id) {
    head = head->next;
    while (head) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return NULL;
}

void hall_report(Hall* head) {
    FILE* file_ptr = fopen(HALL_PATH, "w");
    is_null_pointer(file_ptr);
    head = head->next;

    if (head == NULL)
        fprintf(file_ptr, "there's no hall");

    while (head) {
        fprintf(file_ptr, "%d %s %d\n", head->id, head->name, head->capacity);
        for (int i = 0; i < NUMBER_OF_DAYS; i++) {
            fprintf(file_ptr, "%d %d %d\n", i + 1, head->reserved[i], head->profit[i]);
        }
        head = head->next;
    }
    fclose(file_ptr);
    printf("done\n");
    printf("**************************************");
}

void add_food(Food* array_head[NUMBER_OF_DAYS], Hall* hall_head) {
    char* name = read_word_from_console();
    int food_id, price, capacity, hall_id, day, search_day;
    scanf("%d %d %d %d %d", &food_id, &price, &capacity, &hall_id, &day);
    day--;
    Food* food_ptr = search_in_foods(array_head, food_id, &search_day);

    if (food_ptr != NULL) {
        printf("sorry, that food id is not unique\n");
        printf("**************************************");
    }

    else {
        Hall* hall_ptr = search_in_halls(hall_head, hall_id);
        if (hall_ptr == NULL) {
            printf("sorry that hall hall id doesn't exist\n");
            printf("**************************************");
        }

        else {
            Food* temp = (Food*)malloc(sizeof(Food));
            temp->next = array_head[day]->next;
            array_head[day]->next = temp;

            temp->name = name;
            temp->id = food_id;
            temp->price = price;
            temp->capacity = capacity;
            temp->hall = hall_ptr;
            temp->profit = 0;
            temp->reserved = 0;
            printf("added successfully\n");
            printf("**************************************");
        }
    }
}

Food* search_in_foods(Food* array_head[NUMBER_OF_DAYS], int id, int* day) {
    for (int i = 0; i < NUMBER_OF_DAYS; i++) {
        Food* head = array_head[i]->next;
        while (head) {
            if (head->id == id) {
                *day = i;
                return head;
            }
            head = head->next;
        }
    }
    return NULL;
}

void food_report(Food* array_head[NUMBER_OF_DAYS]) {
    FILE* file_ptr = fopen(FOOD_PATH, "w");
    bool empty = 1;
    for (int i = 0; i < NUMBER_OF_DAYS; i++) {
        Food* head = array_head[i]->next;
        if (head != NULL) {
            empty = 0;
            break;
        }
    }

    if (!empty) {
        for (int i = 0; i < NUMBER_OF_DAYS; i++) {
            // we have to put it days by days
            Food* head = array_head[i]->next;
            while (head) {
                fprintf(file_ptr, "%d %d %d %d\n",
                        head->id, head->capacity, head->reserved, head->profit);
                head = head->next;
            }
        }
    } 
	
	else
        fprintf(file_ptr, "there's no food available");

    fclose(file_ptr);
    printf("done\n");
    printf("**************************************");
}