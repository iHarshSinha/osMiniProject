#include "clientDeclaration.h"


void facultyMenu() {
    while (is_logged_in) {
        clearScreen();
        displayTitle("...Faculty Menu...");
        
        printf("1. Add new Course\n");
        printf("2. Remove offered Course\n");
        printf("3. View enrollments in Courses\n");
        printf("4. View your Courses\n");
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
                displayTitle("Adding a New Course for the students");
                
                char courseCode[20];
                char courseName[MAX_COURSE_NAME_LENGTH];
                int seats;
                
                printf("Enter course code for the new course: ");
                scanf("%s", courseCode);
                getchar(); 
                
                printf("Enter maximum number of students that can register: ");
                scanf("%d", &seats);
                getchar(); 
                
                printf("Enter course name: ");
                fgets(courseName, MAX_COURSE_NAME_LENGTH, stdin);
                
                size_t len = strlen(courseName);
                if (len > 0 && courseName[len-1] == '\n') {
                    courseName[len-1] = '\0';
                }
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d ADD_COURSE %s %d %s", current_user_id, courseCode, seats, courseName);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 2: { 
                clearScreen();
                displayTitle("Remove Course");
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d VIEW_COURSES", current_user_id);
                sendRequest(request, response);
                printf("%s\n", response);
                
                char courseCode[20];
                printf("\nEnter course code to remove from the database: ");
                scanf("%s", courseCode);
                getchar(); 
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d REMOVE_COURSE %s", current_user_id, courseCode);
                sendRequest(request, response);
                
                displaySuccess(response);
                break;
            }
            case 3: { 
                clearScreen();
                displayTitle("Course Enrollments");
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d VIEW_ENROLLMENTS", current_user_id);
                sendRequest(request, response);
                
                printf("%s\n", response);
                waitForEnter();
                break;
            }
            case 4: {
                clearScreen();
                displayTitle("Displaying Courses offered by you");
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d VIEW_COURSES", current_user_id);
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
                
                snprintf(request, BUFFER_SIZE, "FACULTY %d CHANGE_PASSWORD %s %s", current_user_id, oldPassword, newPassword);
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
