#include "serverDeclaration.h"



char *handleAdminRequest(const char *request, int userId)
{
    User *admin = findUserById(userId);
    char *res = (char *)malloc(BUFFER_SIZE);
    if (admin==NULL || admin->type != ADMIN)
    {
        strcpy(res, "Access denied");
        return res;
    }

    char *t = strtok((char *)request, " ");
    if (!t)
    {
        strcpy(res, "Invalid request");
        return res;
    }

    char inp[50];
    strcpy(inp, t);

    if (strcmp(inp, "ADD_STUDENT") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        if (!username || !password)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        if (findUserByUsername(username))
        {
            sprintf(res, "Username %s already used by someone", username);
            return res;
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
        sprintf(res, "Student added with the following details\nUserId: %d", student.id);
    }
    else if (strcmp(inp, "ADD_FACULTY") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        if (!username || !password)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        if (findUserByUsername(username))
        {
            sprintf(res, "Faculty with username %s already exists", username);
            return res;
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
        sprintf(res, "Faculty added with the following details\nUserId: %d", faculty.id);
    }
    else if (strcmp(inp, "TOGGLE_STUDENT") == 0)
    {
        char *studentIdStr = strtok(NULL, " ");
        if (!studentIdStr)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        int studentId = atoi(studentIdStr);
        User *student = findUserById(studentId);
        if (!student || student->type != STUDENT)
        {
            strcpy(res, "Student account not found");
            return res;
        }
        student->active = !student->active;
        saveData();
        sprintf(res, "Student %s %s successfully", student->username,
                student->active ? "activated" : "deactivated");
    }
    else if (strcmp(inp, "UPDATE_USER") == 0)
    {
        char *userIdStr = strtok(NULL, " ");
        char *field = strtok(NULL, " ");
        char *value = strtok(NULL, " ");
        if (!userIdStr || !field || !value)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        int userId = atoi(userIdStr);
        User *user = findUserById(userId);
        if (!user)
        {
            strcpy(res, "User not found");
            return res;
        }
        if (strcmp(field, "password") == 0)
        {
            strncpy(user->password, value, MAX_STR - 1);
            user->password[MAX_STR - 1] = '\0';
            saveData();
            strcpy(res, "Password updated");
        }
        else if (strcmp(field, "username") == 0)
        {
            if (findUserByUsername(value))
            {
                sprintf(res, "Username %s already exists", value);
            }
            else
            {
                strncpy(user->username, value, MAX_STR - 1);
                user->username[MAX_STR - 1] = '\0';
                saveData();
                strcpy(res, "Username updated successfully");
            }
        }
        else
        {
            strcpy(res, "Invalid field to update");
        }
    }
    else if (strcmp(inp, "VIEW_USERS") == 0)
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
        strcpy(res, result);
        free(result);
    }
    else if (strcmp(inp, "VIEW_COURSES") == 0)
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
        strcpy(res, result);
        free(result);
    }
    else
    {
        strcpy(res, "Invalid admin inp");
    }

    return res;
}
