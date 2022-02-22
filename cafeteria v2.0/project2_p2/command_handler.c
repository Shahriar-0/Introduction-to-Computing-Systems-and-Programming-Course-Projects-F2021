//this file is dedicated to implemention of functions for handling commands
#include "command_handler.h"

int entering_page(bool* pointer_to_user_status, User** pointer_to_user, bool* pointer_to_admin_status, User* head_of_users) {
	//this funtion will be called in the beggining of the app or whenever we log out
	//i.e. when we are not a registered member
	char* command = read_word_from_console();
	if (!strcmp(command, EXIT)) {//if we want to end the app
		free(command);
		return TERMINATE;
	}
	if (!strcmp(command, LOGIN)) 
		logging_in(pointer_to_user_status, pointer_to_user, pointer_to_admin_status, head_of_users);
	else if (!strcmp(command, SIGNUP)) 
		signing_up(head_of_users);
	else
		printf("acces denied\n*********************************************\n");
	//if there's something left in the stdin
	empty_stdin();
	//we need to free memory
	free(command);
	return CONTINUE;
}

void empty_stdin() {
	//this one will clear the stdin for us
	//no input
	//no output
	fseek(stdin, 0, SEEK_END);
	int num = ftell(stdin);
	if (num) {
		char c;
		while ((c = getc(stdin)) != '\n' && c != EOF) {}
	}
}

void logging_in(bool* pointer_to_user_status, User** pointer_to_user, bool* pointer_to_admin_status, User* head_of_users) {
	//in here we handle the process of loggin in
	//input:	
		//pointer to user and admin status in source
		//head of the linked list that contains the users
		//pointer to the user in case we login
	//no output
	char* username = read_word_from_console();
	char* password = read_word_from_console();
	if (!strcmp(username, ADMIN_USERNAME) && !strcmp(password, ADMIN_PASS)) {
		//admin is here
		*pointer_to_admin_status = LOGGED_IN;
		printf("welcome admin\n");
	}
	else {
		//we have to check users
		(*pointer_to_user) = search_in_users(head_of_users, username, USERNAME); //we search for usernames
		if ((*pointer_to_user) == NULL)	//the given username doesn't exist
			printf("sorry, we didn't find you in our users:(\ncheck your spelling or signup\n*********************************************\n");
		else {
			//that username exist but we have to check the password as well
			if (!strcmp((*pointer_to_user)->password, password)) {
				//everything is allright
				printf("welcome %s :)\n", (*pointer_to_user)->username);
				*pointer_to_user_status = LOGGED_IN;
			}
			else {
				(*pointer_to_user) = NULL;	//cause password is not right
				printf("please check your password again\n*********************************************\n");
			}
		}
	}
	//we need to free memory
	free(username);
	free(password);
}

void signing_up(User* head) {
	//this one will handle signing up process
	//input:
		//head of the list of the users
	//no output
	char* username = read_word_from_console();
	char* id_string = read_word_from_console();
	char* password = read_word_from_console();
	User* ptr = search_in_users(head, username, USERNAME);
	if (ptr != NULL) { //username is not unique
		printf("an account with that username already exist\n*********************************************\n");
		free(username);	//cause it's not valid we no longer need them
		free(password);
	}
	else {
		ptr = search_in_users(head, id_string, ID);
		if (ptr != NULL) { //id is not unique
			printf("an account with that id already exist\n*********************************************\n");
			free(username);	//cause it's not valid we no longer need them
			free(password);
		}
		else {
			//everything is unique and we have to add it
			User* temp = (User*)malloc(sizeof(User));
			//switch pointer
			temp->next = head->next;
			head->next = temp;
			//add new information
			temp->username = username;
			temp->id = atoi(id_string);
			temp->password = password;
			temp->money = 0;	//we don't have any money yet
			for (int i = 0; i < NUMBER_OF_DAYS; i++)		//we haven't reserved anything yet
				temp->days[i] = (Reserved_food*)create_dummy(RESERVED_FOOD);
			printf("added successfully\n*********************************************\n");
		}
	}
	free(id_string);	//we free it here cause unlike the other two we don't work with that address
						//any more and we have put it in an int
}

User* search_in_users(User* head, char* word, int mode) {
	//we search in the users for the given word
	//input:
		//head of the users list
		//taken word
		//the thing we are searching for
	//output:
		//the pointer to that user or null if we can't find it
	head = head->next;
	if (mode == USERNAME) {
		while (head) {
			if (!strcmp(head->username, word))
				//found it
				return head;
			head = head->next;

		}
	}
	else if (mode == ID) {
		int id = atoi(word);
		while (head) {
			if (head->id == id)
				//found it
				return head;
			head = head->next;
		}
	}
	//nope, not here
	return NULL;
}

int getting_command_from_user(bool* pointer_to_user_status, User** pointer_to_user, Food* array_head_of_food[NUMBER_OF_DAYS], Coupon* head_of_coupons) {
	//this function is dedicated to processing user's orders
	//input:
		//pointer to the user status in source
		//a pointer to the pointer in the source for keeping users address in memory
	//output:
		//whether to continue or shut the app
	char* command = read_word_from_console();
	if (!strcmp(command, EXIT)) {//if we want to end the app
		free(command);
		return TERMINATE;
	}
	//now we check for different orders to react appropriately
	if (!strcmp(command, LOGOUT)) {
		*pointer_to_user_status = LOGGED_OUT;
		(*pointer_to_user) = NULL;
		printf("logged out successfully\n*********************************************\n");
	}
	else if (!strcmp(command, ADD_FOOD) || !strcmp(command, ADD_COUPON) || !strcmp(command, ADD_HALL) || !strcmp(command, FOOD_REPORT) || !strcmp(command, HALL_REPORT))
		//these are for admin
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
	else if (!strcmp(command, DRESERVE_DISCOUNT)) 
		reserve_discount(*pointer_to_user, array_head_of_food, head_of_coupons);
	else if (!strcmp(command, SHOW_RESERVED)) 
		show_reserved(*pointer_to_user);
	else
		printf("not valid\n*********************************************\n");
	//if there's something left in the stdin
	empty_stdin();
	free(command);
	return CONTINUE;
}

void show_menu(Food* array_head[NUMBER_OF_DAYS]) {
	//this one will display the menu for user sorted by days
	//input:	
		//array of heads of food's linked list
	//no putput
	for (int i = 0; i < NUMBER_OF_DAYS; i++) {
		//we print each days
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
	//this will help user reserve food without discount
	//inut:
		//pointer to user that is active right now
		//list of food
	//no output
	int food_id, day;
	scanf("%d", &food_id);
	Food* food_ptr = search_in_foods(array_of_heads_of_foods, food_id, &day);
	if (food_ptr == NULL)	//that food id is wrong
		printf("that food id doesn't exit, please check again\n*********************************************\n");
	else {
		if (food_ptr->capacity == food_ptr->reserved) //the food is no longer availabe
			printf("sorry, we have run out that food\n*********************************************\n");
		else if (food_ptr->hall->capacity == food_ptr->hall->reserved[day]) //hall is full
			printf("sorry, the hall is full\n*********************************************\n");
		else if (pointer_to_user->money < food_ptr->price) //we don't have enough money
			printf("sorry, you don't have enough money\n*********************************************\n");
		else {
			//everything is ok
			Reserved_food* temp = (Reserved_food*)malloc(sizeof(Reserved_food));
			//switch pointers
			temp->next = pointer_to_user->days[day]->next;
			pointer_to_user->days[day]->next = temp;
			//we add new information
			temp->food_name = food_ptr->name;
			temp->hall_name = food_ptr->hall->name;
			temp->price = food_ptr->price;
			temp->food_id = food_id;
			//make some changes in hall and food and our money
			pointer_to_user->money -= food_ptr->price;	// we reduce the price from account
			food_ptr->hall->reserved[day]++;	//one more person is in the hall
			(food_ptr->reserved)++;	//one less food do we have
			food_ptr->profit += food_ptr->price; //we add it to profit
			food_ptr->hall->profit[day] += food_ptr->price;
			printf("reserved succesfully\n*********************************************\n");
		}
	}
}

void reserve_discount(User* pointer_to_user, Food* array_of_heads_of_foods[NUMBER_OF_DAYS], Coupon* head_of_coupons) {
	//this will help user reserve food wit discount
	//inut:
		//pointer to user that is active right now
		//list of food
		//list of coupons
	//no output
	int food_id, coupon_id, day;
	scanf("%d %d", &food_id, &coupon_id);
	Food* food_ptr = search_in_foods(array_of_heads_of_foods, food_id, &day);
	if (food_ptr == NULL)	//that food id is wrong
		printf("that food id doesn't exit, please check again\n*********************************************\n");
	else {
		Coupon* coupon_ptr = search_in_coupons(head_of_coupons, coupon_id);
		if (coupon_ptr == NULL)	//coupon id is wrong
			printf("that coupon id doesn't exit, please check again\n*********************************************\n");
		else {
			if (food_ptr->capacity == food_ptr->reserved) //the food is no longer availabe
				printf("sorry, we have run out that food\n*********************************************\n");
			else if (food_ptr->hall->capacity == food_ptr->hall->reserved[day]) //hall is full
				printf("sorry, the hall is full\n*********************************************\n");
			else if (coupon_ptr->capacity <= 0)	//it's not valid anymore
				printf("sorry, that coupon is no longer valid\n*********************************************\n");
			else if (pointer_to_user->money < (food_ptr->price - (food_ptr->price * coupon_ptr->percantage / 100))) //we don't have enough money
				printf("sorry, you don't have enough money\n*********************************************\n");
			else {
				//everything is ok
				Reserved_food* temp = (Reserved_food*)malloc(sizeof(Reserved_food));
				//switch pointers
				temp->next = pointer_to_user->days[day]->next;
				pointer_to_user->days[day]->next = temp;
				//we add new information
				temp->food_name = food_ptr->name;
				temp->hall_name = food_ptr->hall->name;
				temp->price = food_ptr->price;
				temp->food_id = food_id;
				//make some changes in hall and food and our money
				pointer_to_user->money -= (food_ptr->price - (food_ptr->price * coupon_ptr->percantage / 100));	// we reduce the price from account
				food_ptr->hall->reserved[day]++;	//one more person is in the hall
				(food_ptr->reserved)++;	//one less food do we have
				food_ptr->profit += (food_ptr->price - (food_ptr->price * coupon_ptr->percantage / 100)); //we add it to profit
				food_ptr->hall->profit[day] += (food_ptr->price - (food_ptr->price * coupon_ptr->percantage / 100));
				(coupon_ptr->capacity)--;
				printf("reserved succesfully\n*********************************************\n");
			}
		}
	}
}

void show_reserved(User* pointer_to_user) {
	//this one help us mange our reseved foods
	//input:
		//pointer to the user address in memory
	//no output
	for (int i = 0; i < NUMBER_OF_DAYS; i++) {
		//go through each day and print it
		printf("%d:\n", i + 1);
		Reserved_food* head = pointer_to_user->days[i]->next;
		while(head) {
			printf("%s %d %d %s\n", head->food_name, head->price, head->food_id, head->hall_name);
			head = head->next;
		}
	}
	printf("*********************************************\n");
}

int getting_command_from_admin(bool* pointer_to_admin_status, Food* array_of_head_of_foods[NUMBER_OF_DAYS], Coupon* head_of_coupons, Hall* head_of_halls) {
	//this function is dedicated to processing admin's orders
	//input:
		//head of coupons and halls
		//array which holds linked list of foods
		//pointer to the admin status in source
	//output:
		//whether to continue or shut the app
	char* command = read_word_from_console();
	if (!strcmp(command, EXIT)) { // ending the app
		free(command);
		return TERMINATE;
	}
	//check for different commands
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
		printf("logged out successfully\n*********************************************\n");
	}
	else
		printf("not valid\n*********************************************\n");
	//if there's something left in the stdin
	empty_stdin();
	free(command);
	return CONTINUE;
}

void add_coupon(Coupon* head) {
	//a function to add coupons
	//input:
		//head of the coupons list
	//no output
	int id, percentage, capacity;
	scanf("%d %d %d", &id, &percentage, &capacity);
	Coupon* ptr = search_in_coupons(head, id);
	if (ptr != NULL)	//we already have that id
		printf("that coupon id is not unique\n*********************************************\n");
	else {
		//it's unique so we add it
		Coupon* temp = (Coupon*)malloc(sizeof(Coupon));
		//switch pointer	
		temp->next = head->next;
		head->next = temp;
		//add new information
		temp->id = id;
		temp->percantage = percentage;
		temp->capacity = capacity;
		printf("added successfully\n*********************************************\n");
	}
}

Coupon* search_in_coupons(Coupon* head, int id) {
	//we search in the coupons for the given id
	//input:
		//head of the couponas list
		//taken id
	//output:
		//the pointer to that coupon or null if we can't find it
	head = head->next;
	while (head) {
		if (head->id == id)
			//found it
			return head;
		head = head->next;
	}
	//nope, not here
	return NULL;
}

void add_hall(Hall* head) {
	//a function to add halls
	//input:
		//head of the halls list
	//no output
	char* name = read_word_from_console();
	int id, capacity;
	scanf("%d %d", &id, &capacity);
	Hall* ptr = search_in_halls(head, id);
	if (ptr != NULL)	//we already have that id
		printf("that hall id is not unique\n*********************************************\n");
	else {
		//it's unique so we add it
		Hall* temp = (Hall*)malloc(sizeof(Hall));
		//switch pointer
		temp->next = head->next;
		head->next = temp;
		//add new information
		temp->name = name;
		temp->id = id;
		temp->capacity = capacity;
		for (int i = 0; i < NUMBER_OF_DAYS; i++) {
			//no one has bought anything from here yet
			temp->reserved[i] = 0;
			temp->profit[i] = 0;
		}
		printf("added successfully\n*********************************************\n");
	}
}

Hall* search_in_halls(Hall* head, int id) {
	//we search in the halls for the given id
	//input:
		//head of the halls list
		//taken id
	//output:
		//the pointer to that halls or null if we can't find it
	head = head->next;
	while (head) {
		if (head->id == id)
			//found it
			return head;
		head = head->next;
	}
	//nope, not here
	return NULL;
}

void hall_report(Hall* head) {
	//a function to save datas in a file
	//input:
		//head of the halls linked list
	//no output
	FILE* file_ptr = fopen(HALL_PATH, "w");
	is_null_pointer(file_ptr);
	head = head->next;
	//first we have to check if it's empty
	if (head == NULL) {
		//no hall here
		fprintf(file_ptr, "there's no hall");
	}
	while (head) {
		fprintf(file_ptr, "%d %s %d\n", head->id, head->name, head->capacity);
		for (int i = 0; i < NUMBER_OF_DAYS; i++) {
			fprintf(file_ptr, "%d %d %d\n", i + 1, head->reserved[i], head->profit[i]);
		}
		head = head->next;
	}
	fclose(file_ptr);
	printf("done\n*********************************************\n");
}

void add_food(Food* array_head[NUMBER_OF_DAYS], Hall* hall_head) {
	//a function to add foods
	//input:
		//array of head of foods
		//hall head to check hall id
	//no output
	char* name = read_word_from_console();
	int food_id, price, capacity, hall_id, day, search_day;
	scanf("%d %d %d %d %d", &food_id, &price, &capacity, &hall_id, &day);
	day--;	//to match it with array index
	Food* food_ptr = search_in_foods(array_head, food_id, &search_day); //the last one is not necessary here but we need it for function call
	if (food_ptr != NULL)	//that food id is not unique
		printf("sorry, that food id is not unique\n*********************************************\n");
	else {
		Hall* hall_ptr = search_in_halls(hall_head, hall_id);
		if (hall_ptr == NULL)	//that hall doesn't exist
			printf("sorry that hall hall id doesn't exist\n*********************************************\n");
		else {
			//everything is allright so we add it
			Food* temp = (Food*)malloc(sizeof(Food));
			//switch pointers
			temp->next = array_head[day]->next;
			array_head[day]->next = temp;
			//we add new information
			temp->name = name;
			temp->id = food_id;
			temp->price = price;
			temp->capacity = capacity;
			temp->hall = hall_ptr;
			temp->profit = 0;
			temp->reserved = 0;
			printf("added successfully\n*********************************************\n");
		}
	}
}

Food* search_in_foods(Food* array_head[NUMBER_OF_DAYS], int id, int* day) {
	//we search in the foods for the given id
	//input:
		//array of head of the foods list
		//taken id
		//days in which food exists
	//output:
		//the pointer to that halls or null if we can't find it
	for (int i = 0; i < NUMBER_OF_DAYS; i++) {
		//we should search each days
		Food* head = array_head[i]->next;
		while (head) {
			if (head->id == id) {
				*day = i;
				return head;
			}
			head = head->next;
		}
	}
	//nope, not here
	return NULL;
}

void food_report(Food* array_head[NUMBER_OF_DAYS]) {
	//this will save the datas of foods in a file
	//input:
		//array of heads of linked list
	//no output
	FILE* file_ptr = fopen(FOOD_PATH, "w");
	//first we have to check if it's empty
	bool empty = 1;
	for (int i = 0; i < NUMBER_OF_DAYS; i++) {
		//we gotta check all of the linked lists
		//to make sure it's empty
		Food* head = array_head[i]->next;
		if (head != NULL) {
			//it's not empty
			empty = 0;
			break;
		}
	}
	if (!empty) {
		for (int i = 0; i < NUMBER_OF_DAYS; i++) {
			//we have to put it days by days
			Food* head = array_head[i]->next;
			while (head) {
				fprintf(file_ptr, "%d %d %d %d\n", head->id, head->capacity, head->reserved, head->profit);
				head = head->next;
			}
		}
	}
	else //if it's empty
		fprintf(file_ptr, "there's no food available");
	fclose(file_ptr);
	printf("done\n*********************************************\n");
}