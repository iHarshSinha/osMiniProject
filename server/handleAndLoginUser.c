#include "serverDeclaration.h"

char* loginUser(const char* username, const char* password) {
    char* res = (char*)malloc(BUFFER_SIZE);
    for (int i = 0; i < users_size; i++) {
        // printf("%s %s\n",password,users[i].password);
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
                // printf("here\n");
            if (!users[i].active) {
                strcpy(res, "Account is currently deactivated");
                return res;
            }
            // printf("second here\n");
            const char* userType = users[i].type == ADMIN ? "ADMIN" : 
                                 users[i].type == STUDENT ? "STUDENT" : "FACULTY";
            sprintf(res, "LOGIN_SUCCESS %s %d", userType, users[i].id);
            return res;
        }
    }
    strcpy(res, "Unable to login. Some error Occured");
    return res;
}

void* handleClient(void* client_socket) {
    int sock = *(int*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE] = {0};

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t bytesRead = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytesRead <= 0) {
            close(sock);
            printf("Connection closed with client\n");
            return NULL;
        }

        char* res = processRequest(buffer, sock);

        send(sock, res, strlen(res), 0);
        free(res);

        if (strcmp(buffer, "EXIT") == 0) {
            printf("Client asked to close connection\n");
            close(sock);
            return NULL;
        }
    }

    return NULL;
}