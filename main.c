#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*struct for some information student, room and maintainence*/

typedef struct
{
	int roomNo;
	char type[10];
	int maxOccupants;
	int currentOccupants;
} Room;

typedef struct 
{
	int studentID[20];
	char name[50];
	int level;
} Student;

typedef struct 
{
	int roomNo;
	char issueDescription[100];
	char status[20];
	char severity[20];
} MaintenanceRequest;



int main(void)
{
	int loopStatus = 1;

	do
	{
		int choice;
		printf("\n---------------------------------\n");
		printf("Welcome to Room Allocation System");
		printf("\n---------------------------------\n\n\n");

		printf("Dashboard\n");
		printf("1. Add Student\n");
		printf("2. Add Maintenance Request\n");
		printf("3. Generate Report\n");
		printf("4. Exit\n");
		printf("Enter your choice: ");
		if (scanf("%d", &choice) != 1)
		{
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
			addStudent();
			break;
		case 2:
			addMaintenanceRequest();
			break;
		case 3:
			generateReport();
			break;
		case 4:
			printf("Exiting the program. Goodbye!\n");
			loopStatus = 0;
			break;
		default:
			printf("\nInvalid choice, try again");
			break;
		}
	} while (loopStatus == 1);

	return 0;
}

/* Functions for managing students, rooms, and maintenance requests would go here */

void addStudent()
{
	printf("Add Student Page\n");
}

void addMaintenanceRequest()
{
	printf("Add Maintenance Request Page\n");
}

void generateReport()
{
	printf("Generate Report Page\n");
}

/* Small Operational Functions */

double calculateFee()
{

}

void assignRoom()
{

}

double calculatePenalty()
{

}

void generateReport()
{

}