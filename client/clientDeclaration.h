#ifndef CLIENTDECLARATION_H
#define CLIENTDECLARATION_H

#include "../myincludes.h"

// macros
#define PORT 3000
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_COURSE_NAME_LENGTH 100
#define MAX_RESPONSE_LENGTH 1024

// global variables
extern int client_fd;
extern bool is_logged_in;
extern char current_user_type[20];
extern int current_user_id;



// function declarations
void connectToServer();
void loginMenu();
void adminMenu();
void studentMenu();
void facultyMenu();
void sendRequest(const char* request, char* response);



void clearScreen(void);
void displayTitle(const char* title);
void displayError(const char* message);
void displaySuccess(const char* message);
void waitForEnter(void);
void cleanExit(int signal_num);
void getPasswordInput(char* password);
void extractToken(const char* str, char* result, int token_index);

#endif