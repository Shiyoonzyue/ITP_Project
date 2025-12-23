#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

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
	int studentID;
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


Student students[MAX_STUDENTS];

int studentCount = 0;

void addStudent(void);
void addMaintenanceRequest(void);
void generateReport(void);
double calculateFee(void);
void assignRoom(void);
double calculatePenalty(void);


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
	if (studentCount >= MAX_STUDENTS)
	{
		printf("Cannot add more students. Maximum limit reached.\n");
		return;
	}

	// Input student details
	Student *newStudent = &students[studentCount];

	while (getchar() != '\n'); // Clear input buffer

	printf("Enter Student Name: ");
	fgets(newStudent->name, sizeof(newStudent->name), stdin);
	newStudent->name[strcspn(newStudent->name, "\n")] = '\0';

	printf("Enter Student Level (1/2/3/4): ");
	if(scanf("%d", &newStudent->level) != 1 || newStudent->level < 1 || newStudent->level > 4)
	{
		printf("Invalid input for student level.\n");
		while (getchar() != '\n');
		return;
	}
	
	newStudent->studentID = studentCount + 1;
	studentCount++;
	/* for room allocation and calculate fee and payment status tracking*/

	

	/*--------------------------------------------------------------------*/

	printf("Student added successfully with ID: %d\n", newStudent->studentID);
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
	return 0.0;
}

void assignRoom()
{

}

double calculatePenalty()
{
	return 0.0;
}
