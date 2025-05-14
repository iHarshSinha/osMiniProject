#include "serverDeclaration.h"
char *handleAdminRequest(const char *request, int userId)
{
    char *response = (char *)malloc(BUFFER_SIZE);
    User *admin = findUserById(userId);
    if (!admin || admin->type != ADMIN)
    {
        strcpy(response, "Access denied");
        return response;
    }

    char *token = strtok((char *)request, " ");
    if (!token)
    {
        strcpy(response, "Invalid admin request");
        return response;
    }

    char command[50];
    strcpy(command, token);

    if (strcmp(command, "ADD_STUDENT") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        if (!username || !password)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        if (findUserByUsername(username))
        {
            sprintf(response, "Student with username %s already exists", username);
            return response;
        }
        User student;
        student.id = users_size ? users[users_size - 1].id + 1 : 1;
        strncpy(student.username, username, MAX_STR - 1);
        student.username[MAX_STR - 1] = '\0';
        strncpy(student.password, password, MAX_STR - 1);
        student.password[MAX_STR - 1] = '\0';
        student.type = STUDENT;
        student.active = 1;
        users = (User *)realloc(users, (users_size + 1) * sizeof(User));
        users[users_size++] = student;
        saveData();
        sprintf(response, "Student added successfully with ID %d", student.id);
    }
    else if (strcmp(command, "ADD_FACULTY") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        if (!username || !password)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        if (findUserByUsername(username))
        {
            sprintf(response, "Faculty with username %s already exists", username);
            return response;
        }
        User faculty;
        faculty.id = users_size ? users[users_size - 1].id + 1 : 1;
        strncpy(faculty.username, username, MAX_STR - 1);
        faculty.username[MAX_STR - 1] = '\0';
        strncpy(faculty.password, password, MAX_STR - 1);
        faculty.password[MAX_STR - 1] = '\0';
        faculty.type = FACULTY;
        faculty.active = 1;
        users = (User *)realloc(users, (users_size + 1) * sizeof(User));
        users[users_size++] = faculty;
        saveData();
        sprintf(response, "Faculty added successfully with ID %d", faculty.id);
    }
    else if (strcmp(command, "TOGGLE_STUDENT") == 0)
    {
        char *studentIdStr = strtok(NULL, " ");
        if (!studentIdStr)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        int studentId = atoi(studentIdStr);
        User *student = findUserById(studentId);
        if (!student || student->type != STUDENT)
        {
            strcpy(response, "Student not found");
            return response;
        }
        student->active = !student->active;
        saveData();
        sprintf(response, "Student %s %s successfully", student->username,
                student->active ? "activated" : "deactivated");
    }
    else if (strcmp(command, "UPDATE_USER") == 0)
    {
        char *userIdStr = strtok(NULL, " ");
        char *field = strtok(NULL, " ");
        char *value = strtok(NULL, " ");
        if (!userIdStr || !field || !value)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        int userId = atoi(userIdStr);
        User *user = findUserById(userId);
        if (!user)
        {
            strcpy(response, "User not found");
            return response;
        }
        if (strcmp(field, "password") == 0)
        {
            strncpy(user->password, value, MAX_STR - 1);
            user->password[MAX_STR - 1] = '\0';
            saveData();
            strcpy(response, "Password updated successfully");
        }
        else if (strcmp(field, "username") == 0)
        {
            if (findUserByUsername(value))
            {
                sprintf(response, "Username %s already exists", value);
            }
            else
            {
                strncpy(user->username, value, MAX_STR - 1);
                user->username[MAX_STR - 1] = '\0';
                saveData();
                strcpy(response, "Username updated successfully");
            }
        }
        else
        {
            strcpy(response, "Invalid field to update");
        }
    }
    else if (strcmp(command, "VIEW_USERS") == 0)
    {
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "Users list:\n");
        for (int i = 0; i < users_size; i++)
        {
            const char *userType = users[i].type == ADMIN ? "ADMIN" : users[i].type == STUDENT ? "STUDENT"
                                                                                               : "FACULTY";
            char line[256];
            sprintf(line, "ID: %d, Username: %s, Type: %s, Status: %s\n",
                    users[i].id, users[i].username, userType,
                    users[i].active ? "Active" : "Inactive");
            strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
        }
        strcpy(response, result);
        free(result);
    }
    else if (strcmp(command, "VIEW_COURSES") == 0)
    {
        acquireReadLock(COURSE_FILE);
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "Courses list:\n");
        for (int i = 0; i < courses_size; i++)
        {
            User *faculty = findUserById(courses[i].facultyId);
            char line[256];
            sprintf(line, "ID: %d, Code: %s, Name: %s, Faculty: %s, Seats: %d/%d\n",
                    courses[i].id, courses[i].code, courses[i].name,
                    faculty ? faculty->username : "Unknown",
                    courses[i].enrolledStudents, courses[i].totalSeats);
            strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
        }
        releaseLock(COURSE_FILE);
        strcpy(response, result);
        free(result);
    }
    else
    {
        strcpy(response, "Invalid admin command");
    }

    return response;
}
