#include "command_handler.h"

int main() {
	//let's have some dummies for more convenience
	User* head_of_users = (User*)create_dummy(USER);
	Hall* head_of_halls = (Hall*)create_dummy(HALL);
	Coupon* head_of_coupons = (Coupon*)create_dummy(COUPON);
	Food* array_of_head_of_foods[NUMBER_OF_DAYS];	//we store foods in 7 different linked list separately
	for (int i = 0; i < NUMBER_OF_DAYS; i++)
		array_of_head_of_foods[i] = (Food*)create_dummy(FOOD);		
	//initializing some variables to hold accound status
	bool user_status = LOGGED_OUT, admin_status = LOGGED_OUT;
	//this one will keep the pointer to the users information
	User* pointer_to_user = NULL;
	//load datas from file
	load_data(head_of_users);
	while (1) {
		while (user_status == LOGGED_OUT && admin_status == LOGGED_OUT) {
			//when we are not in as a user or the admin
			int mode = entering_page(&user_status, &pointer_to_user, &admin_status, head_of_users);
			if (mode == TERMINATE) {
				freeing(head_of_users, head_of_coupons, array_of_head_of_foods, head_of_halls);
				exit(EXIT_SUCCESS);
			}
		}
		while (user_status == LOGGED_IN) {
			//when we are a user
			int mode = getting_command_from_user(&user_status, &pointer_to_user, array_of_head_of_foods, head_of_coupons);
			if (mode == TERMINATE) {
				freeing(head_of_users, head_of_coupons, array_of_head_of_foods, head_of_halls);
				exit(EXIT_SUCCESS);
			}
		}
		while (admin_status == LOGGED_IN) {
			//when we are the admin
			int mode = getting_command_from_admin(&admin_status, array_of_head_of_foods, head_of_coupons, head_of_halls);
			if (mode == TERMINATE) {
				freeing(head_of_users, head_of_coupons, array_of_head_of_foods, head_of_halls);
				exit(EXIT_SUCCESS);
			}
		}
	}
}												