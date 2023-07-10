#include "file_handler.h"

void is_null_pointer(void* pointer) {
    if (pointer == NULL) {
        printf("sorry, something wrong has happened");
        exit(EXIT_FAILURE);
    }
}

void* create_dummy(int mode) {
    if (mode == USER) {
        User* ptr = (User*)malloc(sizeof(User));
        ptr->next = NULL;
        return ptr;
    } else if (mode == HALL) {
        Hall* ptr = (Hall*)malloc(sizeof(Hall));
        ptr->next = NULL;
        return ptr;
    } else if (mode == COUPON) {
        Coupon* ptr = (Coupon*)malloc(sizeof(Coupon));
        ptr->next = NULL;
        return ptr;
    } else if (mode == FOOD) {
        Food* ptr = (Food*)malloc(sizeof(Food));
        ptr->next = NULL;
        return ptr;
    } else if (mode == RESERVED_FOOD) {
        Reserved_food* ptr = (Reserved_food*)malloc(sizeof(Reserved_food));
        ptr->next = NULL;
        return ptr;
    }
}

char* read_word_from_file(FILE* pointer_to_file) {
    char letter;
    int word_size = 0;
    char* word = NULL;
    while ((letter = fgetc(pointer_to_file)) != EOF && letter != '\n' && letter != ' ') {
        word_size += sizeof(char);
        word = (char*)realloc(word, word_size);
        *(word + (word_size - 1)) = letter;
    }
    word = (char*)realloc(word, word_size + 1);
    *(word + (word_size)) = NULL;
    return word;
}

void load_data(User* head) {
    FILE* pointer_to_file = fopen(USERS_PATH, "a+");
    is_null_pointer(pointer_to_file);
    char c;
    while ((c = fgetc(pointer_to_file) != EOF)) {
        fseek(pointer_to_file, -1, SEEK_CUR);
        User* temp = (User*)malloc(sizeof(User));
        temp->next = head->next;
        head->next = temp;
        temp->username = read_word_from_file(pointer_to_file);
        temp->id = atoi(read_word_from_file(pointer_to_file));
        temp->password = read_word_from_file(pointer_to_file);
        temp->money = 0;                          // we don't have any money at first

        for (int i = 0; i < NUMBER_OF_DAYS; i++)  // because we haven't reserved anything yet
            temp->days[i] = (Reserved_food*)create_dummy(RESERVED_FOOD);
    }
    fclose(pointer_to_file);
}

char* read_word_from_console() {
    char* word = NULL;
    int word_size = 0;
    char letter = 'a';  // just for the first time we go in the loop
    while ((letter = getc(stdin)) != ' ' && letter != '\n') {
        word_size++;
        word = (char*)realloc(word, word_size * sizeof(char));
        word[word_size - 1] = letter;
    }
    word = (char*)realloc(word, (word_size + 1) * sizeof(char));
    word[word_size] = NULL;  // just to make it like other strings
    return word;
}

void freeing(User* head_of_users, Coupon* head_of_coupons, 
			 Food* array_of_head_of_foods[NUMBER_OF_DAYS], Hall* head_of_halls) {

    FILE* file_ptr = fopen(USERS_PATH, "w");
    is_null_pointer(file_ptr);

    User* next_user = head_of_users->next;
    free(head_of_users);
    head_of_users = next_user;

    while (head_of_users) {
        // but first we save in a file
        fprintf(file_ptr, "%s %d %s\n", head_of_users->username, head_of_users->id, head_of_users->password);

        free(head_of_users->username);
        free(head_of_users->password);
        for (int i = 0; i < NUMBER_OF_DAYS; i++) {
            Reserved_food* reserved_head = head_of_users->days[i];
            while (reserved_head) {
                Reserved_food* next_reserved = reserved_head->next;
                free(reserved_head);
                reserved_head = next_reserved;
            }
        }
        next_user = head_of_users->next;
        free(head_of_users);
        head_of_users = next_user;
    }

    fclose(file_ptr);

    while (head_of_coupons) {
        Coupon* next_coupon = head_of_coupons->next;
        free(head_of_coupons);
        head_of_coupons = next_coupon;
    }

    Hall* next_hall = head_of_halls->next;
    free(head_of_halls);
    head_of_halls = next_hall;
    while (head_of_halls) {
        free(head_of_halls->name);
        next_hall = head_of_halls->next;
        free(head_of_halls);
        head_of_halls = next_hall;
    }

    for (int i = 0; i < NUMBER_OF_DAYS; i++) {
        Food *current_head = array_of_head_of_foods[i], *next_food = current_head->next;
        free(current_head);
        current_head = next_food;
        while (current_head) {
            free(current_head->name);
            next_food = current_head->next;
            free(current_head);
            current_head = next_food;
        }
    }
    printf("hope you enjoyed, bye");
}