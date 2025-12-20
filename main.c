#include <stdio.h>


int main(){	
	mainMenu();
	return 0;
}

void mainMenu(){
	int choice;
	printf("\n---------------------------------\n");
	printf("Welcome to Room Allocation System");
	printf("\n---------------------------------\n\n\n");
	
	printf("Please Login Or Register\n");
	printf("1.Login\n");
	printf("2.Register\n");
	printf("3.Exit\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);
	
	switch(choice){
		case 1:
			login();
			break;
		case 2:
			registerPage();
			break;
		case 3:
			printf("\nExit");
			break;
		default:
			printf("Loop");
			break;
	}
}

void login(){
	printf("\nLogin Page");
}

void registerPage(){
	printf("\nRegister Page");
}