#include "clientDeclaration.h"


void adminMenu() {
    while (is_logged_in) {
        clearScreen();
        displayTitle("...Admin Menu...");
        
        printf("1. Add Student\n");
        printf("2. Add Faculty\n");
        printf("3. Activate/Deactivate Student\n");
        printf("4. Update Student/Faculty details\n");
        printf("5. View All Users\n");
        printf("6. View All Courses\n");
        printf("7. Logout\n");
        printf("8. Exit\n");
        
        printf("\nEnter your choice: ");
        
        int choice;
        scanf("%d", &choice);
        getchar(); 
        
        char request[BUFFER_SIZE];
        char response[BUFFER_SIZE];
        
        switch (choice) {
            case 1: { 
                clearScreen();
                displayTitle("Add New Student");
                
                char username[MAX_USERNAME_LENGTH];
                char password[MAX_PASSWORD_LENGTH];
                
                printf("Enter username of the student: ");
                scanf("%s", username);
                getchar(); 
                
                printf("Enter password for the student: ");
                getPasswordInput(password);
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d ADD_STUDENT %s %s", current_user_id, username, password);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 2: { 
                clearScreen();
                displayTitle("Adding New Faculty in the database");
                
                char username[MAX_USERNAME_LENGTH];
                char password[MAX_PASSWORD_LENGTH];
                
                printf("Enter username for the faculty: ");
                scanf("%s", username);
                getchar(); 
                
                printf("Enter password for the faculty: ");
                getPasswordInput(password);
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d ADD_FACULTY %s %s", current_user_id, username, password);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 3: { 
                clearScreen();
                displayTitle("Activate/Deactivate Student");
                
                int studentId;
                
                printf("Enter student ID to toggle between status: ");
                scanf("%d", &studentId);
                getchar(); 
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d TOGGLE_STUDENT %d", current_user_id, studentId);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 4: { 
                clearScreen();
                displayTitle("Update User Details");
                
                int userId;
                char field[20];
                char value[MAX_PASSWORD_LENGTH];
                
                printf("Enter user ID for updation: ");
                scanf("%d", &userId);
                getchar(); 
                
                printf("What do you want to update (username/password): ");
                scanf("%s", field);
                getchar(); 
                
                if (strcmp(field, "password") == 0) {
                    printf("Enter new password: ");
                    getPasswordInput(value);
                } else {
                    printf("Enter new value: ");
                    scanf("%s", value);
                    getchar(); 
                }
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d UPDATE_USER %d %s %s", current_user_id, userId, field, value);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 5: { 
                clearScreen();
                displayTitle("Following is the list of All Users");
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d VIEW_USERS", current_user_id);
                sendRequest(request, response);
                
                printf("%s\n", response);
                waitForEnter();
                break;
            }
            case 6: {
                clearScreen();
                displayTitle("following is the list of All Courses");
                
                snprintf(request, BUFFER_SIZE, "ADMIN %d VIEW_COURSES", current_user_id);
                sendRequest(request, response);
                
                printf("%s\n", response);
                waitForEnter();
                break;
            }
            case 7: {
                is_logged_in = false;
                strcpy(current_user_type, "");
                current_user_id = -1;
                printf("Logged out successfully.\n");
                waitForEnter();
                return; 
            }
            case 8: { 
                sendRequest("EXIT", response);
                cleanExit(0);
                break;
            }
            default:
                displayError("Invalid choice!");
        }
    }
}
