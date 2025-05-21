#include "clientDeclaration.h"


void connectToServer() {
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        fprintf(stderr, "Socket creation error\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address or address not supported\n");
        exit(EXIT_FAILURE);
    }
    
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Connection failed: Server might be offline\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to Academia.com Server\n");
}

void sendRequest(const char* request, char* response) {
    memset(response, 0, BUFFER_SIZE);
    
    if (send(client_fd, request, strlen(request), 0) < 0) {
        fprintf(stderr, "Failed to send request\n");
        strcpy(response, "ERROR");
        return;
    }
    
    ssize_t bytesRead = read(client_fd, response, BUFFER_SIZE - 1);
    
    if (bytesRead < 0) {
        fprintf(stderr, "Failed to read response\n");
        strcpy(response, "ERROR");
        return;
    }
    
    response[bytesRead] = '\0';
}

void extractToken(const char* str, char* result, int token_index) {
    char temp[BUFFER_SIZE];
    strcpy(temp, str);
    
    char* token = strtok(temp, " ");
    int i = 0;
    
    while (token != NULL && i < token_index) {
        token = strtok(NULL, " ");
        i++;
    }
    
    if (token != NULL) {
        strcpy(result, token);
    } else {
        result[0] = '\0';
    }
}
