#include "clientDeclaration.h"


void clearScreen() {
    printf("\033[2J\033[1;1H"); 
}

void displayTitle(const char* title) {
    printf("====================================\n");
    printf("      %s\n", title);
    printf("====================================\n");
}

void displayError(const char* message) {
    printf("\033[31m%s\033[0m\n", message); 
    printf("Press Enter to continue...");
    getchar(); 
}

void displaySuccess(const char* message) {
    printf("\033[32m%s\033[0m\n", message); 
    printf("Press Enter to continue...");
    getchar(); 
}

void waitForEnter() {
    printf("\nPress Enter to continue...");
    getchar(); 
}


void cleanExit(int signal_num) {
    printf("\nExiting client application...\n");
    if (client_fd != -1) {
        close(client_fd);
    }
    exit(signal_num);
}

void getPasswordInput(char* password) {
    struct termios old_tio, new_tio;
    
    tcgetattr(STDIN_FILENO, &old_tio);
    
    new_tio = old_tio;
    
    new_tio.c_lflag &= (~ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) != NULL) {
        size_t len = strlen(password);
        if (len > 0 && password[len-1] == '\n') {
            password[len-1] = '\0';
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}
