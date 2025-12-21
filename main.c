#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
	char studentID[20];
	char password[20];
	char name[50];
	int level;
} Student;

void mainMenu(void);
void login(void);
void registerPage(void);



void mainMenu(void)
{
	int status = 1;

	do
	{
		int choice;
		printf("\n---------------------------------\n");
		printf("Welcome to Room Allocation System");
		printf("\n---------------------------------\n\n\n");

		printf("Please Login Or Register\n");
		printf("1. Login\n");
		printf("2. Register\n");
		printf("3. Exit\n");
		printf("Enter your choice: ");
		if (scanf("%d", &choice) != 1)
		{
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		switch (choice)
		{
		case 1:
			login();
			break;
		case 2:
			registerPage();
			break;
		case 3:
			status = 0;
			break;
		default:
			printf("\nInvalid choice, try again");
			break;
		}
	} while (status == 1);
}

void login(void)
{
	printf("\nLogin Page\n");
	printf("Enter Student ID: ");

	

}

void registerPage(void)
{
	printf("\nRegister Page");
}

int main(void)
{
	mainMenu();
	return 0;
}

// try comments for github