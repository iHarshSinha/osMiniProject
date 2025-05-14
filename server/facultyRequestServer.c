#include "serverDeclaration.h"
char *handleFacultyRequest(const char *request, int userId)
{
    char *response = (char *)malloc(BUFFER_SIZE);
    User *faculty = findUserById(userId);
    if (!faculty || faculty->type != FACULTY)
    {
        strcpy(response, "Access denied");
        return response;
    }

    char *token = strtok((char *)request, " ");
    if (!token)
    {
        strcpy(response, "Invalid faculty request");
        return response;
    }

    char command[50];
    strcpy(command, token);

    if (strcmp(command, "ADD_COURSE") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        char *seatsStr = strtok(NULL, " ");
        char *courseName = strtok(NULL, "");
        if (!courseCode || !seatsStr || !courseName)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        int seats = atoi(seatsStr);
        acquireWriteLock(COURSE_FILE);
        for (int i = 0; i < courses_size; i++)
        {
            if (strcmp(courses[i].code, courseCode) == 0)
            {
                releaseLock(COURSE_FILE);
                sprintf(response, "Course with code %s already exists", courseCode);
                return response;
            }
        }
        Course course;
        course.id = courses_size ? courses[courses_size - 1].id + 1 : 1;
        strncpy(course.code, courseCode, MAX_STR - 1);
        course.code[MAX_STR - 1] = '\0';
        strncpy(course.name, courseName, MAX_STR - 1);
        course.name[MAX_STR - 1] = '\0';
        course.facultyId = faculty->id;
        course.totalSeats = seats;
        course.enrolledStudents = 0;
        courses = (Course *)realloc(courses, (courses_size + 1) * sizeof(Course));
        courses[courses_size++] = course;
        saveData();
        releaseLock(COURSE_FILE);
        sprintf(response, "Course added successfully: %s - %s", courseCode, courseName);
    }
    else if (strcmp(command, "REMOVE_COURSE") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        if (!courseCode)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        acquireWriteLock(COURSE_FILE);
        acquireWriteLock(ENROLLMENT_FILE);
        int courseId = -1;
        for (int i = 0; i < courses_size; i++)
        {
            if (strcmp(courses[i].code, courseCode) == 0 && courses[i].facultyId == faculty->id)
            {
                courseId = courses[i].id;
                for (int j = i; j < courses_size - 1; j++)
                {
                    courses[j] = courses[j + 1];
                }
                courses_size--;
                courses = (Course *)realloc(courses, courses_size * sizeof(Course));
                break;
            }
        }
        if (courseId == -1)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Course not found or you don't have permission to remove it");
            return response;
        }
        int new_size = 0;
        for (int i = 0; i < enrollments_size; i++)
        {
            if (enrollments[i].courseId != courseId)
            {
                enrollments[new_size] = enrollments[i];
                new_size++;
            }
        }
        enrollments_size = new_size;
        enrollments = (Enrollment *)realloc(enrollments, enrollments_size * sizeof(Enrollment));
        saveData();
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        sprintf(response, "Course %s removed successfully", courseCode);
    }
    else if (strcmp(command, "VIEW_ENROLLMENTS") == 0)
    {
        acquireReadLock(COURSE_FILE);
        acquireReadLock(ENROLLMENT_FILE);
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "Course enrollments:\n");
        int hasCourses = 0;
        for (int i = 0; i < courses_size; i++)
        {
            if (courses[i].facultyId == faculty->id)
            {
                char line[256];
                sprintf(line, "\nCourse: %s - %s\nEnrolled students: %d/%d\n",
                        courses[i].code, courses[i].name,
                        courses[i].enrolledStudents, courses[i].totalSeats);
                strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
                int hasStudents = 0;
                for (int j = 0; j < enrollments_size; j++)
                {
                    if (enrollments[j].courseId == courses[i].id)
                    {
                        User *student = findUserById(enrollments[j].studentId);
                        if (student)
                        {
                            sprintf(line, "- %s (ID: %d)\n", student->username, student->id);
                            strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
                            hasStudents = 1;
                        }
                    }
                }
                if (!hasStudents)
                {
                    strncat(result, "- No students enrolled yet\n",
                            BUFFER_SIZE - strlen(result) - 1);
                }
                hasCourses = 1;
            }
        }
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        if (!hasCourses)
        {
            strcpy(response, "You have not offered any courses");
        }
        else
        {
            strcpy(response, result);
        }
        free(result);
    }
    else if (strcmp(command, "VIEW_COURSES") == 0)
    {
        acquireReadLock(COURSE_FILE);
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "Your courses:\n");
        int hasCourses = 0;
        for (int i = 0; i < courses_size; i++)
        {
            if (courses[i].facultyId == faculty->id)
            {
                char line[256];
                sprintf(line, "Code: %s, Name: %s, Enrollment: %d/%d\n",
                        courses[i].code, courses[i].name,
                        courses[i].enrolledStudents, courses[i].totalSeats);
                strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
                hasCourses = 1;
            }
        }
        releaseLock(COURSE_FILE);
        if (!hasCourses)
        {
            strcpy(response, "You have not offered any courses");
        }
        else
        {
            strcpy(response, result);
        }
        free(result);
    }
    else if (strcmp(command, "CHANGE_PASSWORD") == 0)
    {
        char *oldPassword = strtok(NULL, " ");
        char *newPassword = strtok(NULL, " ");
        if (!oldPassword || !newPassword)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        if (strcmp(faculty->password, oldPassword) != 0)
        {
            strcpy(response, "Incorrect current password");
            return response;
        }
        strncpy(faculty->password, newPassword, MAX_STR - 1);
        faculty->password[MAX_STR - 1] = '\0';
        saveData();
        strcpy(response, "Password changed successfully");
    }
    else
    {
        strcpy(response, "Invalid faculty command");
    }

    return response;
}
