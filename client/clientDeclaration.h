#ifndef CLIENTDECLARATION_H
#define CLIENTDECLARATION_H

#include "../myincludes.h"

// macros
#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_COURSE_NAME_LENGTH 100
#define MAX_RESPONSE_LENGTH 1024

// global variables
int client_fd = -1;
bool is_logged_in = false;
char current_user_type[20] = "";
int current_user_id = -1;



// function declarations



#endif