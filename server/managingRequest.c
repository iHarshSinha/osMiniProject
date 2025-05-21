#include "serverDeclaration.h"


char *processRequest(const char *request, int clientSocket)
{
    char *res = (char *)malloc(BUFFER_SIZE);
    res[0] = '\0';

    char *t = strtok((char *)request, " ");
    if (!t)
    {
        strcpy(res, "Invalid request");
        return res;
    }

    char inp[50];
    strcpy(inp, t);

    if (strcmp(inp, "LOGIN") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        if (username && password)
        {
            strcpy(res, loginUser(username, password));
        }
        else
        {
            strcpy(res, "Invalid login format");
        }
    }
    else if (strcmp(inp, "ADMIN") == 0 || strcmp(inp, "STUDENT") == 0 ||
             strcmp(inp, "FACULTY") == 0)
    {
        char *idInString = strtok(NULL, " ");
        if (!idInString)
        {
            strcpy(res, "Invalid request format");
            return res;
        }
        int userId = atoi(idInString);
        char *subRequest = strtok(NULL, "");
        if (!subRequest)
            subRequest = "";

        if (strcmp(inp, "ADMIN") == 0)
        {
            strcpy(res, handleAdminRequest(subRequest, userId));
        }
        else if (strcmp(inp, "STUDENT") == 0)
        {
            strcpy(res, handleStudentRequest(subRequest, userId));
        }
        else
        {
            strcpy(res, handleFacultyRequest(subRequest, userId));
        }
    }
    else
    {
        strcpy(res, "Invalid request format");
    }

    return res;
}
