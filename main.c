#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sets of CONSTANS for maximum students and maintenance
#define MAX_STUDENTS 100
#define MAX_MAINTENANCE 100

/*struct for some information student, room and maintainence*/


// structure for Room
typedef struct
{
	int roomNo;
	char type[10];	  // Single, Double, Quad
	int maxOccupants; // 1, 2, 4
	int currentOccupants;
} Room;

// structure for student
typedef struct
{
	int studentID;
	char name[50];
	int level;
	int roomNo;
	double montlyFees; // NEW
	int paymentStatus; // NEW
} Student;

// structure for maintanance request
typedef struct
{
	int roomNo;
	char issueDescription[100];
	char status[20];
	char severity[20];
} MaintenanceRequest;

Student students[MAX_STUDENTS];
MaintenanceRequest maintenance[MAX_MAINTENANCE];
int totalMaintenance = 0;

Room rooms[6] =
	{
		{
			101,
			"Single",
			1,
			1,
		},
		{102, "Single", 1, 1},
		{201, "Double", 2, 1},
		{202, "Double", 2, 0},
		{301, "Quad", 4, 1},
		{302, "Quad", 4, 1}};

int totalStudents = 0;

// funtion prototype here
void addStudent(void);
void addMaintenanceRequest(void);
void generateReport(void);
void studentList(void);
double calculateFee(void);
void assignRoom(void);
double calculatePenalty(void);
int findRoomIndex(int roomNo);

// function for file
void saveStudentsToFile(void);
void loadStudentsFromFile(void);
void saveMaintenanceToFile(void);
void loadMaintenanceFromFile(void);

int main(void)
{
	loadStudentsFromFile();
	loadMaintenanceFromFile();    // load the saved file
	
	int loopStatus = 1;

	Student s1 = {1001, "Ridhwan", 1, 101};
	Student s2 = {1002, "Yazid", 3, 102};
	Student s3 = {1003, "Harry", 4, 201};
	Student s4 = {1004, "Syamim", 2, 302};
	Student s5 = {1005, "Nadzrul", 2, 301};

	students[totalStudents++] = s1;
	students[totalStudents++] = s2;
	students[totalStudents++] = s3;
	students[totalStudents++] = s4;
	students[totalStudents++] = s5;

	printf("\n\n\n---------------------------------\n");
	printf("Welcome to Room Allocation System");
	printf("\n---------------------------------\n");

	do
	{
		int choice;

		printf("\n\n\nDashboard\n");
		printf("1. Add Student\n");
		printf("2. Add Maintenance Request\n");
		printf("3. Generate Report\n");
		printf("4. Student List\n");
		printf("5. Exit\n");
		printf("6. View Maintenance List\n");
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
			addStudent();
			break;
		case 2:
			addMaintenanceRequest();
			break;
		case 3:
			generateReport();
			break;
		case 4:
			studentList();
			break;
		case 5:
			saveStudentsToFile();
			saveMaintenanceToFile();
			printf("Exiting the program. Goodbye ta ta!\n");
			loopStatus = 0;
			break;
			
		case 6:
			viewMaintenanceList();
			break;
			
		default:
			printf("\nInvalid choice, try again");
			break;
		}
	} while (loopStatus == 1);

	return 0;
}

/* Functions for managing students, rooms, and maintenance requests would go here */

void addStudent(void)
{
	if (totalStudents >= MAX_STUDENTS)
	{
		printf("Cannot add more students. Maximum limit reached.\n");
		return;
	}

	// Input student details
	Student *newStudent = &students[totalStudents];

	while (getchar() != '\n')
		; // Clear input buffer

	printf("Enter Student Name: ");
	fgets(newStudent->name, sizeof(newStudent->name), stdin);
	newStudent->name[strcspn(newStudent->name, "\n")] = '\0';

	printf("Enter Student Level (1/2/3/4): ");
	if (scanf("%d", &newStudent->level) != 1 || newStudent->level < 1 || newStudent->level > 4)
	{
		printf("Invalid input for student level.\n");
		while (getchar() != '\n')
			;
		return;
	}

	assignRoom();
	/*--------------------------------------------------------------------*/
	/* for calculate fee and payment status tracking*/
	// use calculateFee() function here
	// use calculatePenalty() function here

	/*--------------------------------------------------------------------*/

	newStudent->studentID = 1000 + totalStudents + 1;
	totalStudents++;
	printf("Student added successfully with ID: %d\n", newStudent->studentID);
}

void studentList(void)
{
	int i;
	printf("\n\nStudent List Page\n");
	printf("------------------------------------------------------------\n");
	printf("%-6s %-15s %-8s %-10s %-10s\n", "ID", "Name", "Level", "Room No", "Room Type");
	printf("------------------------------------------------------------\n");

	for (i = 0; i < totalStudents; i++)
	{
		int roomIndex = findRoomIndex(students[i].roomNo);

		printf("%-6d %-15s %-8d %-10d ",
			   students[i].studentID,
			   students[i].name,
			   students[i].level,
			   students[i].roomNo);

		if (roomIndex != -1)
		{
			printf("%-10s\n", rooms[roomIndex].type);
		}
		else
		{
			printf("%-10s\n", "Not Assigned");
		}
	}
}

void addMaintenanceRequest(void)
{
	if (totalMaintenance >= MAX_MAINTENANCE){
	
	printf("Cannot add more maintenance request. You have reached Maximum request. \n");
	return;
}

MaintenanceRequest *m = &maintenance[totalMaintenance];

printf("Enter Your Room Number : ");
if (scanf(%d , &m->roomNo) != 1){
	printf("invalid input.\n");
	while(getchar() != '\n')  //clear the buffer
	return;
}

while(getchar() != '\n');  //clear the buffer

printf("Enter Issue Description : ");
fgets(m->issueDescription , sizeof(m->issueDescription ), stdin);
m->issueDescription [strcspn(m->issueDescription , "\n")] = '\0';

printf("Enter the severity (Low/medium/High): ");
scanf("%19s", m->severity);

strcpy(m->issueDescription status, "pending "); //to default status

totalmaintenance++;
printf("Maintenance request added successfully ! \n");
}

void viewMaintenanceList(void){
	if(totalMaintenance == 0){
		printf("No maintenance records found.\n");
		return; 
		
}
printf("\nMaintenance Requests\n");
    printf("---------------------------------------------------------\n");
    printf("%-8s %-30s %-10s %-12s\n", "RoomNo", "Issue", "Severity", "Status");
    printf("---------------------------------------------------------\n");
	
	for (int i = 0; i< totalMaintenance; i++){
		 printf("%-8d %-30s %-10s %-12s\n",
               maintanance[i].roomNo,
               maintanance[i].issueDescription,
               maintanance[i].severity,
               maintanance[i].status);
	}
}
		
		

void generateReport()
{
	printf("Generate Report Page\n");
}

void assignRoom()
{
	int studentIndex = totalStudents;
	int assigned = 0;
	printf("Please assign a room to the student.\n");

	// Display the available rooms based on type
	while (!assigned)
	{
		char roomType[10];
		printf("Enter Room Type (Single/Double/Quad): ");
		scanf("%s", roomType);

		printf("Available Rooms of type %s:\n", roomType);
		int found = 0;
		for (int i = 0; i < 6; i++)
		{
			if (strcmp(rooms[i].type, roomType) == 0 && rooms[i].currentOccupants < rooms[i].maxOccupants)
			{
				printf("Room No: %d, Current Occupants: %d/%d\n", rooms[i].roomNo, rooms[i].currentOccupants, rooms[i].maxOccupants);
				found = 1;
			}
		}

		if (!found)
		{
			printf("No available rooms of type %s. Please choose another type.\n", roomType);
			continue;
		}

		// Prompt user to assign a room for student
		int chosenRoomNo;
		printf("Enter Room No to assign: ");
		if (scanf("%d", &chosenRoomNo) != 1)
		{
			printf("Invalid input for room number.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		// Validate the chosen room
		int roomIndex = findRoomIndex(chosenRoomNo);
		if (roomIndex == -1 || strcmp(rooms[roomIndex].type, roomType) != 0 || rooms[roomIndex].currentOccupants >= rooms[roomIndex].maxOccupants)
		{
			printf("Room %d is not valid or full. Please try again.\n");
			continue;
		}

		students[studentIndex].roomNo = chosenRoomNo;
		rooms[roomIndex].currentOccupants++;
		assigned = 1;
		printf("Room %d assigned to student %s successfully.\n", chosenRoomNo, students[studentIndex].name);
	}
}

/* Small Operational Functions */

double calculateFee()
{
	return 0.0;
}

double calculatePenalty()
{
	return 0.0;
}

int findRoomIndex(int roomNo)
{
	for (int i = 0; i < 6; i++)
	{
		if (rooms[i].roomNo == roomNo)
		{
			return i;
		}
	}
	return -1;
}

int isRoomAvailable(Room *r)
{
	return r->currentOccupants < r->maxOccupants;
}

// file funtion definition (save file)
void saveStudentsToFile(void)
{
	FILE *fp = fopen("students.txt", "w"); // set pointer called fp then make a file name students.txt

	// condition for error
	if (fp == NULL)
	{
		printf("ERROR OPENING STUDENTS.TXT\n");
		return;
	}

	// the for loop is important to goes through every students data start from the first student[0], until the last student
	for (int i = 0; i < totalStudents; i++)
	{
		// format in table
		fprintf(fp, "%d | %s | %d | %.2f | %s \n",
				students[i].studentID,
				students[i].name,
				students[i].roomNo,
				students[i].montlyFees,
				students[i].paymentStatus == 1 ? "Paid" : "Unpaid");
	}

	// close the file and send succes message
	fclose(fp);
	printf("STUDENT DATA SAVED SUCCESSFULLY\n");
}

// file function definition (load file)
void loadStudentsFromFile(void)
{
	FILE *fp = fopen("students.txt", "r");

	if (fp == NULL)
	{
		return;
	}

	totalStudents = 0;

	while (fscanf(fp, "%d | %49[^|] | %d | %d |%lf | %s \n",
				  &students[totalStudents].studentID,
				  &students[totalStudents].name,
				  &students[totalStudents].level,
				  &students[totalStudents].roomNo,
				  &students[totalStudents].montlyFees,
				  &students[totalStudents].paymentStatus) == 6)
	{
		totalStudents++;
	}

	fclose(fp);
}