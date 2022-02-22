#include "file_handler.h"

void is_null_pointer(void* pointer) {
	//to avoid runtime error, we exit app immediately
	//input:
		//a pointer
	//no output
	if (pointer == NULL) {
		printf("sorry, something wrong has happened");
		exit(EXIT_FAILURE);
	}
}

void* create_dummy(int mode) {
	//to create dummy pointers for our linked list
	//input:
		//the mode(i.e. which kind of pointer do we want)
	//output:
		//the created pointer
	if (mode == USER) {
		User* ptr = (User*)malloc(sizeof(User));
		ptr->next = NULL;
		return ptr;
	}
	else if (mode == HALL) {
		Hall* ptr = (Hall*)malloc(sizeof(Hall));
		ptr->next = NULL;
		return ptr;
	}
	else if (mode == COUPON) {
		Coupon* ptr = (Coupon*)malloc(sizeof(Coupon));
		ptr->next = NULL;
		return ptr;
	}
	else if (mode == FOOD) {
		Food* ptr = (Food*)malloc(sizeof(Food));
		ptr->next = NULL;
		return ptr;
	}
	else if (mode == RESERVED_FOOD) {
		Reserved_food* ptr = (Reserved_food*)malloc(sizeof(Reserved_food));
		ptr->next = NULL;
		return ptr;
	}
}

char* read_word_from_file(FILE* pointer_to_file) {
	//this will read a single word from file
	//input:
		//pointer to a file from which we want to read
	//output:
		//the address of the read word
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
	//load datas from file file and put it in our linked list
	//input:
		//head of the linked list
	//no output
	FILE* pointer_to_file = fopen(USERS_PATH, "a+");
	is_null_pointer(pointer_to_file);
	char c;
	while ((c = fgetc(pointer_to_file) != EOF)) {
		fseek(pointer_to_file, -1, SEEK_CUR);
		User* temp = (User*)malloc(sizeof(User));
		//we change the next pointers to add it in our linked list
		temp->next = head->next;
		head->next = temp;
		temp->username = read_word_from_file(pointer_to_file);
		temp->id = atoi(read_word_from_file(pointer_to_file));
		temp->password = read_word_from_file(pointer_to_file);
		temp->money = 0;	//we don't have any money
		for (int i = 0; i < NUMBER_OF_DAYS; i++) //because we haven't reserved anythin yet
			temp->days[i] = (Reserved_food*)create_dummy(RESERVED_FOOD);
	}
	fclose(pointer_to_file);
}

char* read_word_from_console() {
	//to read a command from console
	//no input
	//output:
		//address of the stored string
	char* word = NULL;
	int word_size = 0;
	char letter = 'a';	//just for the first time we go in the loop
	while ((letter = getc(stdin)) != ' ' && letter != '\n'){
		word_size++;
		word = (char*)realloc(word, word_size * sizeof(char));
		word[word_size - 1] = letter;
	}
	word = (char*)realloc(word, (word_size + 1) * sizeof(char));
	word[word_size] = NULL;		//just to make it like other strings
	return word;
}

void freeing(User* head_of_users, Coupon* head_of_coupons, Food* array_of_head_of_foods[NUMBER_OF_DAYS], Hall* head_of_halls) {
	//this one will be called before ending app to free memories
	//input:
		//every list that we have allocated throughout the program
	//no output
	//we have to save users first
	FILE* file_ptr = fopen(USERS_PATH, "w");
	is_null_pointer(file_ptr);
	//cause we have a dummy
	User* next_user = head_of_users->next;
	free(head_of_users);
	head_of_users = next_user;
	while (head_of_users) {
		//but first we save in a file
		fprintf(file_ptr, "%s %d %s\n", head_of_users->username, head_of_users->id, head_of_users->password);
		//free string
		free(head_of_users->username);
		free(head_of_users->password);
		//free the block that we allocated to reserved foods
		for (int i = 0; i < NUMBER_OF_DAYS; i++) {
			Reserved_food* reserved_head = head_of_users->days[i];
			while (reserved_head) {
				Reserved_food* next_reserved = reserved_head->next;
				free(reserved_head);
				reserved_head = next_reserved;
			}
		}
		//free block and move forward
		next_user = head_of_users->next;
		free(head_of_users);
		head_of_users = next_user;
	}
	fclose(file_ptr);
	//now coupons
	while (head_of_coupons) {
		//cause we don't have string here we ignore dummy
		Coupon* next_coupon = head_of_coupons->next;
		free(head_of_coupons);
		head_of_coupons = next_coupon;
	}
	//now halls
	//cause we have a dummy
	Hall* next_hall = head_of_halls->next;
	free(head_of_halls);
	head_of_halls = next_hall;
	while (head_of_halls) {
		//free string
		free(head_of_halls->name);
		//free block and move forward
		next_hall = head_of_halls->next;
		free(head_of_halls);
		head_of_halls = next_hall;
	}
	//now foods
	//cause we have stored it in an array it's a little bit different
	for (int i = 0; i < NUMBER_OF_DAYS; i++) {
		//we go through each day
		Food* current_head = array_of_head_of_foods[i], * next_food = current_head->next;
		free(current_head);
		current_head = next_food;
		while (current_head) {
			//free string
			free(current_head->name);
			//free block and move forward
			next_food = current_head->next;
			free(current_head);
			current_head = next_food;
		}
	}
	printf("hope you enjoyed, bye");
}