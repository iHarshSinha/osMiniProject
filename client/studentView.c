#include "clientDeclaration.h"


void studentMenu() {
    while (is_logged_in) {
        clearScreen();
        displayTitle("...Student Menu...");
        
        printf("1. Enroll to new Courses\n");
        printf("2. Unenroll from already enrolled Courses\n");
        printf("3. View enrolled Courses\n");
        printf("4. View all available Courses\n");
        printf("5. Change Password\n");
        printf("6. Logout\n");
        printf("7. Exit\n");
        
        printf("\nEnter your choice: ");
        
        int choice;
        scanf("%d", &choice);
        getchar(); 
        
        char request[BUFFER_SIZE];
        char response[BUFFER_SIZE];
        
        switch (choice) {
            case 1: { 
                clearScreen();
                displayTitle("Enrolling you to a New Course");
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d VIEW_COURSES", current_user_id);
                sendRequest(request, response);
                printf("%s\n", response);
                
                char courseCode[20];
                printf("\nEnter course code to enroll: ");
                scanf("%s", courseCode);
                getchar(); 
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d ENROLL %s", current_user_id, courseCode);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 2: { 
                clearScreen();
                displayTitle("Unenroll from Course");
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d VIEW_ENROLLED", current_user_id);
                sendRequest(request, response);
                printf("%s\n", response);
                
                char courseCode[20];
                printf("\nEnter course code from which you want to unenroll: ");
                scanf("%s", courseCode);
                getchar(); 
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d UNENROLL %s", current_user_id, courseCode);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 3: { 
                clearScreen();
                displayTitle("List of courses you are enrolled into");
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d VIEW_ENROLLED", current_user_id);
                sendRequest(request, response);
                
                printf("%s\n", response);
                waitForEnter();
                break;
            }
            case 4: { 
                clearScreen();
                displayTitle("List of All Available Courses");
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d VIEW_COURSES", current_user_id);
                sendRequest(request, response);
                
                printf("%s\n", response);
                waitForEnter();
                break;
            }
            case 5: {
                clearScreen();
                displayTitle("Change Password");
                
                char oldPassword[MAX_PASSWORD_LENGTH];
                char newPassword[MAX_PASSWORD_LENGTH];
                
                printf("Enter current password: ");
                getPasswordInput(oldPassword);
                
                printf("\nEnter new password: ");
                getPasswordInput(newPassword);
                
                snprintf(request, BUFFER_SIZE, "STUDENT %d CHANGE_PASSWORD %s %s", current_user_id, oldPassword, newPassword);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 6: {
                is_logged_in = false;
                strcpy(current_user_type, "");
                current_user_id = -1;
                printf("Logged out successfully.\n");
                waitForEnter();
                return;
            }
            case 7: {
                sendRequest("EXIT", response);
                cleanExit(0);
                break;
            }
            default:
                displayError("Invalid choice!");
        }
    }
}