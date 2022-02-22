#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct student {
	char* first_name;
	char* last_name;
	int student_number;
	float mid_term_exam_score;
	float final_exam_score;
	float homework_score;
	struct student* next;
}Student;

void read_word_from_console(char** word) {
	*word = NULL;
	int word_size = 0;
	char letter = 'a';
	while (letter != ' ' && letter != '\n') {
		scanf("%c", &letter);
		word_size++;
		*word = (char*)realloc(*word, word_size);
		*(*word + word_size - 1) = letter;
	}
	*(*word + word_size - 1) = NULL;
}

char* read_word_from_file(FILE* pointer_to_file) {
	char letter;
	int word_size = 0;
	char *word = NULL;
	while ((letter = fgetc(pointer_to_file)) != EOF && letter != '\n' && letter != ' ') {
		word_size += sizeof(char);
		word = (char*)realloc(word, word_size);
		*(word + (word_size - 1)) = letter;
	}
	word = (char*)realloc(word, word_size + 1); 
	*(word + (word_size)) = NULL;
	return word;
}

void load(FILE* pointer_to_file, Student* head) {
	char c;
	while ((c = fgetc(pointer_to_file) != EOF)) {
		fseek(pointer_to_file, -1, SEEK_CUR);
		Student* temp = (Student*)malloc(sizeof(Student));
		head->next = temp;
		temp->next = NULL;
		head = temp;
		temp->first_name = read_word_from_file(pointer_to_file);
		temp->last_name = read_word_from_file(pointer_to_file);
		temp->student_number = atoi(read_word_from_file(pointer_to_file));
		temp->mid_term_exam_score = atof(read_word_from_file(pointer_to_file));
		temp->final_exam_score = atof(read_word_from_file(pointer_to_file));
		temp->homework_score = atof(read_word_from_file(pointer_to_file));
	}
}

Student* create_dummy() {
	Student* head = (Student*)malloc(sizeof(Student));
	head->next = NULL;
	return head;
}

int end(Student* ptr) {
	return !(bool)(ptr->next);
}

int add_to_i(Student* head, Student* new_std, int i) {
	while (i--) {
		head = head->next;
		if (!head)
			return 0;
	}
	new_std->next = head->next;
	head->next = new_std;
	return 1;
}

void print_list(Student* head) {
	if (head->next == NULL) {
		printf("empty\n");
		return;
	}
	int i = 0;
	head = head->next;
	while (head) {
		printf("{%d}\nname = %s, last name = %s\nid = %d, midterm score = %.2f, final exam score = %.2f, homework score = %.2f\n"
			, i, head->first_name, head->last_name, head->student_number,head->mid_term_exam_score, head->final_exam_score, head->homework_score);
		i++;
		head = head->next;
	}
}

void read_student(Student* ptr) {
	read_word_from_console(&ptr->first_name);
	read_word_from_console(&ptr->last_name);
	scanf("%d %f %f %f", &ptr->student_number, &ptr->mid_term_exam_score, &ptr->final_exam_score, &ptr->homework_score);
}

Student* search(Student* head, int id) {
	if (head->next == NULL)
		return NULL;
	head = head->next;
	while (head) {
		if (head->student_number == id)
			return head;
		head = head->next;
	}
	return NULL;
}

int delete(Student* head, int id) {
	if (head->next == NULL)
		return 0;
	Student* prev = head;
	head = head->next;
	while (head) {
		if (head->student_number == id) {
			prev->next = head->next;
			free(head->first_name);
			free(head->last_name);
			free(head);
			return 1;
		}
		head = head->next;
		prev = prev->next;
	}
	return 0;
}

int print_reverse(Student* head, int i) {
	if (head == NULL)
		return 0;
	print_reverse(head->next, i + 1);
	printf("{%d}\nname = %s, last name = %s\nid = %d, midterm score = %.2f, final exam score = %.2f, homework score = %.2f\n"
		, i, head->first_name, head->last_name, head->student_number, head->mid_term_exam_score, head->final_exam_score, head->homework_score);
	return 1;
}

int sort_id(Student* head) {
	if (head->next == NULL)
		return 0;
	Student* sorted_head = head->next;
	while (sorted_head->next) {
		Student* iteration_head = head, * element = sorted_head->next;
		while (element->student_number > iteration_head->next->student_number && iteration_head != sorted_head) {
			iteration_head = iteration_head->next;
		}
		if (iteration_head != sorted_head) {
			sorted_head->next = element->next;
			element->next = iteration_head->next;
			iteration_head->next = element;
		}
		else 
			sorted_head = sorted_head->next;
	}
	return 1;
}

int name_comparison(char* x, char* y) {
	int i = 0;
	for (; y[i] != NULL && x[i] != NULL; i++) {
		if (x[i] > y[i])
			return 1;
		else if (x[i] < y[i])
			return 0;
	}
	return x[i] > y[i];
}

int sort_name(Student* head) {
	if (head->next == NULL)
		return 0;
	Student* sorted_head = head->next;
	while (sorted_head->next) {
		Student* iteration_head = head, * element = sorted_head->next;
		while (name_comparison(element->last_name, iteration_head->next->last_name) && iteration_head != sorted_head) {
			iteration_head = iteration_head->next;
		}
		if (iteration_head != sorted_head) {
			sorted_head->next = element->next;
			element->next = iteration_head->next;
			iteration_head->next = element;
		}
		else
			sorted_head = sorted_head->next;
	}
	return 1;
}

int delete_all(Student* head) {
	if (head == NULL)
		return 0;
	delete_all(head->next);
	free(head->first_name);
	free(head->last_name);
	free(head);
	return 1;
}

int length(Student* head) {
	if (head == NULL)
		return 0;
	return 1 + length(head->next);
}

Student* reverse(Student* head) {
	if (head->next == NULL)
		return head;
	Student* tail = reverse(head->next);
	head->next->next = head;
	head->next = NULL;
	return tail;
}

int main() {
	Student* head = create_dummy();
	FILE* file_ptr = fopen("input.txt", "r+");
	load(file_ptr, head);
	fclose(file_ptr);
	char* command = NULL;
	while (1) {
		fflush(stdin);
		read_word_from_console(&command);
		if (!strcmp(command, "add")) {
			Student* ptr = create_dummy();
			read_student(ptr);
			int n;
			scanf("%d", &n);
			int mode = add_to_i(head, ptr, n);
			if (mode)
				printf("done\n");
			else
				printf("index has exceeded the limit\n");
		}
		else if (!strcmp(command, "print"))
			print_list(head);
		else if (!strcmp(command, "end?")) {
			int index;
			Student* temp = head->next;
			scanf("%d", &index);
			while (index && temp) {
				temp = temp->next;
				index--;
			}
			if (index > 0 || (index == 0 && temp == NULL))
				printf("exceeded the number of members\n");
			else
				printf(end(temp) ? "yes\n" : "no\n");
		}
		else if (!strcmp(command, "find")) {
			int id;
			scanf("%d", &id);
			Student* temp = search(head, id);
			if (!temp)
				printf("that student doesnt exist\n");
			else
				printf("these are the information of the given id\nfirst name = %s, last name = %s\nmid term score = %.2f, final exam score = %.2f, homework's score = %.2f\n",
					temp->first_name, temp->last_name, temp->mid_term_exam_score, temp->final_exam_score, temp->homework_score);
		}
		else if (!strcmp(command, "delete")) {
			int id;
			scanf("%d", &id);
			int mode = delete(head, id);
			if (mode)
				printf("done\n");
			else
				printf("that id does not exist\n");
		}
		else if (!strcmp(command, "print_reverse")) {
			int mode = print_reverse(head->next, 0);
			if (!mode)
				printf("it's empty\n");
		}
		else if (!strcmp(command, "sort_id")) {
			int mode = sort_id(head);
			if (mode)
				printf("done\n");
			else
				printf("it's empty or has only one element\n");
		}
		else if (!strcmp(command, "sort_name")) {
			int mode = sort_name(head);
			if (mode)
				printf("done\n");
			else
				printf("it's empty or has only one element\n");
		}
		else if (!strcmp(command, "delete_all")) {
			int mode = delete_all(head->next);
			head->next = NULL;
			if (mode)
				printf("done\n");
			else
				printf("it's empty\n");
		}
		else if (!strcmp(command, "reverse")) {
			if (head->next) {
				head->next = reverse(head->next);
				printf("done\n");
			}
			else
				printf("it's empty\n");
		}
		else if (!strcmp(command, "length?")) 
			printf("%d\n", length(head->next));
		else if (!strcmp(command, "clear"))
			system("cls");
		else if(strcmp(command, ""))
			printf("wrong\n");
	}
	return 0;
}