#include <stdio.h>

void mainMenu(void);
void login(void);
void registerPage(void);

void mainMenu(void){
    int choice;
    printf("\n---------------------------------\n");
    printf("Welcome to Room Allocation System");
    printf("\n---------------------------------\n\n\n");

    printf("Please Login Or Register\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
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
            printf("\nInvalid choice");
            break;
    }
}

void login(void){
    printf("\nLogin Page");
}

void registerPage(void){
    printf("\nRegister Page");
}

int main(void){	
    mainMenu();
    return 0;
}
