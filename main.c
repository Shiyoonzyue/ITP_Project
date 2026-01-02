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
	double monthlyFees; // student base fee
	int paymentStatus;	// 0 = Unpaid, 1 = Paid
	double feesPaid;
} Student;

// structure for maintanance request
typedef struct
{
	int roomNo;
	char issueDescription[100];
	char status[20];   // Pending, In Progress, Completed
	char severity[20]; // Low, Medium, High
} MaintenanceRequest;

// Array to store all maintenance requests
// totalMaintenance keeps track of how many requests have been added

Student students[MAX_STUDENTS];
MaintenanceRequest maintenance[MAX_MAINTENANCE];
int totalMaintenance = 0;

Room rooms[6] =
	{
		{101, "Single", 1, 0},
		{102, "Single", 1, 0},
		{201, "Double", 2, 0},
		{202, "Double", 2, 0},
		{301, "Quad", 4, 0},
		{302, "Quad", 4, 0}};

int totalStudents = 0;

// funtion prototype here
void addStudent(void);
void addMaintenanceRequest(void);
void generateReport(void);
void studentList(void);
double calculateFee(int roomNo);
void assignRoom(void);
void paymentStatusUpdate(void);
double calculatePenalty(double baseFee, int daysOverdue);
int findRoomIndex(int roomNo);
void viewMaintenanceList(void);
int isRoomAvailable(Room *r);
int hasStudentInRoom(int roomNo);

// function for file
void updateMaintenanceStatus(void);
void saveMaintenanceToFile(void);
void loadMaintenanceFromFile(void);
void loadStudentsFromFile(void);
void saveStudentsToFile(void);

int main(void)
{
	loadStudentsFromFile();
	printf("Loaded %d students from file.\n", totalStudents);
	loadMaintenanceFromFile();
	printf("Loaded %d maintenance records from file.\n", totalMaintenance);
	// loadMaintenanceFromFile();    // load the saved file

	int loopStatus = 1;

	printf("\n\n\n---------------------------------\n");
	printf("Welcome to Room Allocation System");
	printf("\n---------------------------------\n");

	do
	{
		int choice;

		printf("\n\n\nDashboard\n");
		printf("1. Student List\n");
		printf("2. Add Student\n");
		printf("3. View Maintenance List\n");
		printf("4. Add Maintenance Request\n");
		printf("5. Update Maintenance Status\n");
		printf("6. Payment Status Update\n");
		printf("7. Generate Report\n");
		printf("8. Exit\n");
		printf("Enter your choice (1-8): ");
		if (scanf("%d", &choice) != 1)
		{
			printf("Invalid input. Please enter a number between 1 to 8.\n");
			while (getchar() != '\n')
				;
			continue;
		}
		// switch case for menu choice
		switch (choice)
		{
		case 1:
			studentList();
			break;
		case 2:
			addStudent();
			saveStudentsToFile();
			break;
		case 3:
			viewMaintenanceList();
			break;
		case 4:
			addMaintenanceRequest();
			saveMaintenanceToFile();
			break;
		case 5:
			updateMaintenanceStatus();
			saveMaintenanceToFile();
			break;
		case 6:
			paymentStatusUpdate();
			saveStudentsToFile();
			break;
		case 7:
			generateReport();
			break;
		case 8:
			saveStudentsToFile();
			saveMaintenanceToFile();
			printf("Exiting the program. Goodbye ta ta!\n");
			loopStatus = 0;
			break;
		default:
			printf("\nInvalid choice, please enter 1-8 to continue...\n");
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

	printf("Enter Student Name (Enter 0 to cancel): ");
	fgets(newStudent->name, sizeof(newStudent->name), stdin);
	newStudent->name[strcspn(newStudent->name, "\n")] = '\0';

	if (strcmp(newStudent->name, "0") == 0)
	{
		printf("\nStudent addition cancelled.\n");
		return;
	}

	while (1)
	{
		printf("\nEnter Student Level (1/2/3/4): ");
		if (scanf("%d", &newStudent->level) != 1 || newStudent->level < 1 || newStudent->level > 4)
		{
			printf("Invalid input for student level.\n");
			while (getchar() != '\n')
				;
			continue;
		}
		break;
	}

	assignRoom();

	newStudent->studentID = 1000 + totalStudents + 1;
	totalStudents++;
	printf("\nStudent added successfully with ID: %d\n", newStudent->studentID);
}

void studentList(void)
{
	if (totalStudents == 0)
	{
		printf("\nNo students found in the system.\n");
		return;
	}

	printf("\n%-6s %-20s %-8s %-10s %-10s %-10s %-14s %-14s\n", "ID", "Name", "Level", "Room No", "Type", "Fees", "Status", "Fees Paid (including penalty)");
	printf("-----------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < totalStudents; i++)
	{
		int roomIndex = findRoomIndex(students[i].roomNo);
		char *type = (roomIndex != -1) ? rooms[roomIndex].type : "N/A";
		char *payment = (students[i].paymentStatus == 1) ? "Paid" : "Unpaid";

		printf("%-6d %-20s %-8d %-10d %-10s %-10.2f %-14s %-14.2f\n",
			   students[i].studentID,
			   students[i].name,
			   students[i].level,
			   students[i].roomNo,
			   type,
			   students[i].monthlyFees,
			   payment,
			   students[i].feesPaid);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------\n");
}
int hasStudentInRoom(int roomNo)
{ // before add maintenance check student in room new function
	for (int i = 0; i < totalStudents; i++)
	{
		if (students[i].roomNo == roomNo)
		{
			return 1; // student available
		}
	}
	return 0; // no student
}

void addMaintenanceRequest(void)
{
	if (totalMaintenance >= MAX_MAINTENANCE)
	{
		printf("Cannot add more maintenance requests.\n");
		return;
	}

	MaintenanceRequest *m = &maintenance[totalMaintenance];
	int roomIndex = -1;

	// check for valid room only for available student
	while (1)
	{
		printf("\n\nEnter Room Number (Enter 0 to cancel): ");
		if (scanf("%d", &m->roomNo) != 1)
		{
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		if (m->roomNo == 0)
		{
			printf("Maintenance request cancelled.\n");
			return;
		}

		roomIndex = findRoomIndex(m->roomNo);

		if (roomIndex == -1)
		{
			printf("Room number does not exist. Try again.\n");
		}
		else if (!hasStudentInRoom(m->roomNo))
		{
			printf("No student registered in this room. Maintenance request is not allowed.\n");
		}
		else
		{
			break;
		}
	}

	while (getchar() != '\n')
		;

	// description for issue
	printf("Enter Issue Description: ");
	fgets(m->issueDescription, sizeof(m->issueDescription), stdin);
	m->issueDescription[strcspn(m->issueDescription, "\n")] = '\0';

	// validation for severity
	while (1)
	{
		printf("Enter Severity (Low / Medium / High): ");
		scanf("%19s", m->severity);

		if (strcmp(m->severity, "Low") == 0 ||
			strcmp(m->severity, "Medium") == 0 ||
			strcmp(m->severity, "High") == 0)
		{
			break;
		}

		printf("Invalid severity. Please enter Low, Medium, or High only.\n");
	}

	// output status
	strcpy(m->status, "Pending");

	totalMaintenance++;

	printf("\nMaintenance request added successfully.\n");
}

// Check if there are any maintenance requests
// If none, print message and return

void viewMaintenanceList(void) // list dalam table student maintenance list
{
	if (totalMaintenance == 0)
	{
		printf("No maintenance records found.\n");
		return;
	}
	printf("\nMaintenance Requests\n");
	printf("---------------------------------------------------------\n");
	printf("%-8s %-30s %-10s %-12s\n", "RoomNo", "Issue", "Severity", "Status");
	printf("---------------------------------------------------------\n");

	for (int i = 0; i < totalMaintenance; i++)
	{
		printf("%-8d %-30s %-10s %-12s\n",
			   maintenance[i].roomNo,
			   maintenance[i].issueDescription,
			   maintenance[i].severity,
			   maintenance[i].status);
	}
}

void assignRoom()
{
	int studentIndex = totalStudents;
	int assigned = 0;
	printf("\nPlease assign a room to the student.\n");

	// Display the available rooms based on type
	while (!assigned)
	{
		char roomType[10];
		printf("\nEnter Room Type (Single/Double/Quad): ");
		scanf("%9s", roomType);

		printf("\nAvailable Rooms of type %s:\n", roomType);
		printf("-----------------------------------------\n");
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
			printf("\nNo available rooms of type %s. Please choose another type.\n", roomType);
			printf("-----------------------------------------\n");
			continue;
		}
		printf("-----------------------------------------\n");
		// Prompt user to assign a room for student
		int chosenRoomNo;
		printf("\nEnter Room No to assign: ");
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
			printf("\nRoom %d is not valid or full. Please try again.\n", chosenRoomNo);
			continue;
		}

		assigned = 1;

		if (strcmp(roomType, "Single") == 0)
		{
			students[studentIndex].monthlyFees = 350.0;
		}
		else if (strcmp(roomType, "Double") == 0)
		{
			students[studentIndex].monthlyFees = 250.0;
		}
		else if (strcmp(roomType, "Quad") == 0)
		{
			students[studentIndex].monthlyFees = 150.0;
		}
		students[studentIndex].paymentStatus = 0; // Unpaid by default
		students[studentIndex].feesPaid = 0.0;
		students[studentIndex].roomNo = chosenRoomNo;
		rooms[roomIndex].currentOccupants++;
		printf("Room %d assigned to student %s successfully.\n", chosenRoomNo, students[studentIndex].name);
	}
}

void paymentStatusUpdate(void)
{
	// Function to update payment status of a student
	int studentID;
	while (1)
	{
		while (1)
		{
			printf("\nEnter Student ID to update payment status (Enter 0 to cancel): ");
			if (scanf("%d", &studentID) != 1)
			{
				printf("Invalid input. Please enter a number.\n");
				while (getchar() != '\n')
					;
				continue;
			}

			if (studentID == 0)
			{
				printf("Payment status update cancelled.\n");
				return;
			}

			break;
		}

		int found = 0; // Search for the student by ID
		for (int i = 0; i < totalStudents; i++)
		{
			if (students[i].studentID == studentID)
			{
				found = 1;
				if (students[i].paymentStatus == 1)
				{
					printf("\nStudent %s has already paid their fees.\n", students[i].name);
				}
				else
				{
					int daysOverdue;
					printf("\nCurrent payment status: Unpaid\n");
					printf("Student name: %s\n", students[i].name);
					printf("Room No: %d\n", students[i].roomNo);
					printf("Room Type: ");
					int roomIndex = findRoomIndex(students[i].roomNo);
					if (roomIndex != -1)
					{
						printf("%s\n", rooms[roomIndex].type);
					}
					else
					{
						printf("N/A\n");
					}
					while (1)
					{
						printf("\nEnter payment overdue in days: ");
						if (scanf("%d", &daysOverdue) != 1 || daysOverdue < 0)
						{
							printf("Invalid input for days overdue.\n");
							while (getchar() != '\n')
								;
							continue;
						}
						break;
					}
					printf("\nOriginal Monthly Fees: RM %.2f\n", students[i].monthlyFees);
					printf("Days Overdue: %d days\n", daysOverdue);
					printf("Penalty Applied: RM %.2f\n\n", calculatePenalty(students[i].monthlyFees, daysOverdue));
					printf("Total Amount: RM %.2f\n", students[i].monthlyFees + calculatePenalty(students[i].monthlyFees, daysOverdue));
					int confirm;
					while (1)
					{
						printf("Confirm Payment? (1 for Yes / 0 for No): ");
						if (scanf("%d", &confirm) != 1 || (confirm != 0 && confirm != 1))
						{
							printf("Invalid input for confirmation.\n");
							while (getchar() != '\n')
								;
							continue;
						}
						break;
					}
					if (confirm == 0)
					{
						printf("Payment update cancelled.\n");
						return;
					}
					else
					{
						students[i].feesPaid = students[i].monthlyFees + calculatePenalty(students[i].monthlyFees, daysOverdue);
						students[i].paymentStatus = 1; // Mark as paid
						printf("Payment status for student %s updated to Paid successfully.\n", students[i].name);
					}
				}
				return;
			}
			else
			{
				found = 0;
			}
		}
		if (!found)
		{
			printf("Student ID %d not found.\n", studentID);
			continue;
		}
	}
}
/* Small Operational Functions */

double calculatePenalty(double baseFee, int daysOverdue)
{
	if (daysOverdue > 7 && daysOverdue <= 30)
	{
		return baseFee * 0.05; // 5% penalty
	}
	else if (daysOverdue > 30)
	{
		return baseFee * 0.10; // 10% penalty
	}
	else
	{
		return 0.0; // No penalty
	}
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

void loadMaintenanceFromFile(void)
{

	FILE *fp = fopen("maintenance.txt", "r");
	if (fp == NULL)
		return; // file belum wujud

	totalMaintenance = 0;

	while (totalMaintenance < MAX_MAINTENANCE)
	{

		// baca room number
		if (fscanf(fp, "%d\n", &maintenance[totalMaintenance].roomNo) != 1)
			break;

		// baca issue description
		fgets(maintenance[totalMaintenance].issueDescription,sizeof(maintenance[totalMaintenance].issueDescription), fp);
		maintenance[totalMaintenance].issueDescription[strcspn(maintenance[totalMaintenance].issueDescription, "\n")] = '\0';

		// baca severity
		fgets(maintenance[totalMaintenance].severity, sizeof(maintenance[totalMaintenance].severity), fp);
		maintenance[totalMaintenance].severity[strcspn(maintenance[totalMaintenance].severity, "\n")] = '\0';

		//baca status
		fgets(maintenance[totalMaintenance].status, sizeof(maintenance[totalMaintenance].status), fp);
		maintenance[totalMaintenance].status[strcspn(maintenance[totalMaintenance].status, "\n")] = '\0';

		totalMaintenance++;
	}

	fclose(fp);
}

void saveMaintenanceToFile(void)
{ // Save Maintenance request to Maintenance File

	FILE *fp = fopen("maintenance.txt", "w");
	if (fp == NULL)
	{

		printf("ERROR OPENING MAINTENANCE FILE\n");
		return;
	}

	for (int i = 0; i < totalMaintenance; i++)
	{

		fprintf(fp, "%d\n%s\n%s\n%s\n",
				maintenance[i].roomNo,
				maintenance[i].issueDescription,
				maintenance[i].severity,
				maintenance[i].status);
	}

	fclose(fp);
}

void updateMaintenanceStatus(void)
{
	if (totalMaintenance == 0)
	{
		printf("No maintenance records to update.\n");
		return;
	}

	int roomNo;

	while (1)
	{
		printf("Enter Room Number to update status (Enter 0 to cancel): ");
		if (scanf("%d", &roomNo) != 1)
		{
			printf("Invalid input. Please enter other number.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		if (roomNo == 0)
		{
			printf("Update maintenance status cancelled.\n");
			return;
		}

		int indexes[MAX_MAINTENANCE];
		int count = 0;

		for (int i = 0; i < totalMaintenance; i++)
		{
			if (maintenance[i].roomNo == roomNo)
			{
				indexes[count++] = i;
			}
		}

		if (count == 0)
		{
			printf("\nNo maintenance records found for Room %d.\n", roomNo);
			continue;
		}

		printf("Maintenance Records for Room %d:\n", roomNo);
		printf("-----------------------------------------------------------------------------------------\n");
		for (int i = 0; i < count; i++)
		{
			int idx = indexes[i];
			printf("%d. Issue: %-35s | Severity: %-10s | Status: %s\n",
				   i + 1,
				   maintenance[idx].issueDescription,
				   maintenance[idx].severity,
				   maintenance[idx].status);
		}
		printf("-----------------------------------------------------------------------------------------\n");
		int choice;
		while (1)
		{
			printf("\nSelect issue to update (1-%d) (Enter 0 to cancel): ", count);
			if (scanf("%d", &choice) != 1)
			{
				printf("Invalid choice. Please try again.\n");
				while (getchar() != '\n')
					;
				continue;
			}

			if (choice == 0)
			{
				printf("Update cancelled. No changes made.\n");
				return;
			}

			if (choice < 1 || choice > count)
			{
				printf("Choice out of range. Please try again.\n");
				continue;
			}
			break;
		}

		int selectedIndex = indexes[choice - 1];
		char newStatus[20];

		printf("Current Status: %s\n", maintenance[selectedIndex].status);
		printf("Enter new status (Pending/In Progress/Completed) (Enter 0 to cancel): ");
		while (getchar() != '\n')
			;
		fgets(newStatus, sizeof(newStatus), stdin);
		newStatus[strcspn(newStatus, "\n")] = '\0';

		if (strcmp(maintenance[selectedIndex].status, "Completed") == 0)
		{
			printf("Maintenance issue for Room %d already Completed.\n", roomNo);
			continue;
		}
		else if (strcmp(newStatus, "0") == 0)
		{
			printf("Update cancelled. No changes made.\n");
			break;
		}
		else
		{
			if (strcmp(newStatus, "Completed") == 0)
			{
				printf("Maintenance issue marked as Completed.\n");
				strcpy(maintenance[selectedIndex].status, newStatus);
			}
			else if (strcmp(newStatus, "In Progress") == 0)
			{
				printf("Maintenance issue marked as In Progress.\n");
				strcpy(maintenance[selectedIndex].status, newStatus);
			}
			else if (strcmp(newStatus, "Pending") == 0)
			{
				printf("Maintenance issue remains Pending.\n");
				strcpy(maintenance[selectedIndex].status, newStatus);
			}
			else
			{
				printf("Invalid status entered. No changes made.\n");
			}
			break;
		}
	}
}

// ------------------------------FILE HANDLING SECTION---------------------------
//  file funtion definition (save file)
void saveStudentsToFile(void)
{
	FILE *fp = fopen("students.txt", "w"); // create student.txt and set pointer called fp
	// if file failed to open, this error pop up in terminal
	if (fp == NULL)
	{
		printf("ERROR OPENING STUDENTS.TXT\n");
		return; // stop
	}

	for (int i = 0; i < totalStudents; i++)
	{
		// Save ID, then the name on a new line, then the numbers
		fprintf(fp, "%d\n%s\n%d %d %.2f %d %.2f\n",
				students[i].studentID,
				students[i].name,
				students[i].level,
				students[i].roomNo,
				students[i].monthlyFees,
				students[i].paymentStatus,
				students[i].feesPaid);
	}
	fclose(fp);
}

// file function definition (load file)
void loadStudentsFromFile(void)
{
	FILE *fp = fopen("students.txt", "r");
	if (fp == NULL)
		return;

	totalStudents = 0;
	for (int i = 0; i < 6; i++)
		rooms[i].currentOccupants = 0;

	while (totalStudents < MAX_STUDENTS)
	{
		// Read ID
		if (fscanf(fp, "%d\n", &students[totalStudents].studentID) != 1)
			break;

		// Read Name (reads until newline)
		fgets(students[totalStudents].name, sizeof(students[totalStudents].name), fp);
		students[totalStudents].name[strcspn(students[totalStudents].name, "\n")] = '\0';

		// Read the remaining numeric fields
		if (fscanf(fp, "%d %d %lf %d %lf\n",
				   &students[totalStudents].level,
				   &students[totalStudents].roomNo,
				   &students[totalStudents].monthlyFees,
				   &students[totalStudents].paymentStatus,
				   &students[totalStudents].feesPaid) != 5)
			break;

		// Update room occupants count based on loaded data
		int idx = findRoomIndex(students[totalStudents].roomNo);
		if (idx != -1)
			rooms[idx].currentOccupants++;

		totalStudents++;
	}
	fclose(fp);
}

// generate report function
void generateReport()
{
	FILE *fp = fopen("report.txt", "w"); // create report.txt and set pointer called fp
	if (fp == NULL)						 // if file failed to open
	{
		printf("ERROR CREATING REPORT FILE\n");
		return; // stop function
	}

	else // iffile create successfully
	{
		// header
		fprintf(fp, "ROOM ALLOCATION SYSTEM REPORT\n");
		fprintf(fp, "------------------------------------\n\n");

		double totalRevenue = 0.0; // to store total revenue

		// this section is for loop all students available
		for (int i = 0; i < totalStudents; i++) // loop all students i
		{
			if (students[i].paymentStatus == 1)			 // only count if student paid
			{											 // PAID
				totalRevenue += students[i].monthlyFees; // add each paid students fee to total
			}
		}

		// body 1 total monthly
		fprintf(fp, "1. TOTAL MONTHLY REVENUE\n");
		fprintf(fp, "------------------------------------\n");
		fprintf(fp, "Total revenue: RM %.2f\n\n", totalRevenue); // display total revenue

		// body 2 students with unpaid
		fprintf(fp, "2. STUDENTS WITH UNPAID FEES\n");
		fprintf(fp, "------------------------------------\n");

		int unpaidFound = 0; // to store unpaid students

		for (int i = 0; i < totalStudents; i++) // loop students
		{
			if (students[i].paymentStatus == 0) // unpaid studnts to print into table
			{									// unpaid
				// printf unpaid students info
				fprintf(fp, "ID: %d | Name: %s | Room: %d | Fee: RM %.2f\n",
						students[i].studentID,
						students[i].name,
						students[i].roomNo,
						students[i].monthlyFees);

				unpaidFound = 1; // mark at least one students have paid
			}
		}

		// if there is no unpaid students then print this message
		if (!unpaidFound)
		{
			fprintf(fp, "All students have paid their fees.\n");
		}
		fprintf(fp, "\n");

		// body 3 room with most maintenance
		fprintf(fp, "3. ROOM WITH MOST MAINTENANCE ISSUES\n");
		fprintf(fp, "-----------------------------------\n");

		if (totalMaintenance == 0)
		{
			fprintf(fp, "No maintenance records available.\n");
		}

		else
		{
			int roomIssueCount[6] = {0}; // array to count issues each room

			// convert room number into array and count each problem room
			for (int i = 0; i < totalMaintenance; i++)
			{
				int idx = findRoomIndex(maintenance[i].roomNo);
				if (idx != -1)
				{
					roomIssueCount[idx]++;
				}
			}

			int maxIssues = 0; // store numer of issues
			int maxRoomIndex = -1;

			// maintenance check
			for (int i = 0; i < 6; i++) // loop
			{
				if (roomIssueCount[i] > maxIssues)
				{
					maxIssues = roomIssueCount[i]; // assign maximum room issue
					maxRoomIndex = i;
				}
			}

			if (maxRoomIndex != -1)
			{
				fprintf(fp, "Room Number: %d\n", rooms[maxRoomIndex].roomNo);
				fprintf(fp, "Total Issue: %d\n\n", maxIssues);
			}
		}

		fclose(fp); // close then a message
		printf("\nReport generated succesfully\n");
	}
}