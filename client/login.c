#include "clientDeclaration.h"

void loginMenu() {
    while (!is_logged_in) {
        clearScreen();
        displayTitle("Academia.com - Login Page");
        
        char username[MAX_USERNAME_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
        char request[BUFFER_SIZE];
        char response[BUFFER_SIZE];
        
        printf("Enter your Username: ");
        scanf("%s", username);
        getchar(); 
        
        printf("Enter your Password: ");
        getPasswordInput(password);
        
        snprintf(request, BUFFER_SIZE, "LOGIN %s %s", username, password);
        sendRequest(request, response);
        
        if (strncmp(response, "LOGIN_SUCCESS", 13) == 0) {
            is_logged_in = true;
            
            char user_type[20];
            extractToken(response, user_type, 1);
            strcpy(current_user_type, user_type);
            
            char user_id[10];
            extractToken(response, user_id, 2);
            current_user_id = atoi(user_id);
            
            displaySuccess("Login successful! Moving to Menu");
        } else {
            char error_msg[BUFFER_SIZE];
            strcpy(error_msg, "Login failed: ");
            
            char* pos = strchr(response, ' ');
            if (pos != NULL) {
                strcat(error_msg, pos + 1);
            } else {
                strcat(error_msg, "Unknown error");
            }
            
            displayError(error_msg);
        }
    }
    
    if (strcmp(current_user_type, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(current_user_type, "STUDENT") == 0) {
        studentMenu();
    } else if (strcmp(current_user_type, "FACULTY") == 0) {
        facultyMenu();
    }
}
