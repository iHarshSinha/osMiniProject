#include "serverDeclaration.h"
char *handleStudentRequest(const char *request, int userId)
{
    char *response = (char *)malloc(BUFFER_SIZE);
    User *student = findUserById(userId);
    if (!student || student->type != STUDENT)
    {
        strcpy(response, "Access denied");
        return response;
    }

    char *token = strtok((char *)request, " ");
    if (!token)
    {
        strcpy(response, "Invalid student request");
        return response;
    }

    char command[50];
    strcpy(command, token);

    if (strcmp(command, "ENROLL") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        if (!courseCode)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        acquireWriteLock(COURSE_FILE);
        acquireWriteLock(ENROLLMENT_FILE);
        Course *course = findCourseByCode(courseCode);
        if (!course)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Course not found");
            return response;
        }
        if (isEnrolled(student->id, course->id))
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Already enrolled in this course");
            return response;
        }
        if (course->enrolledStudents >= course->totalSeats)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Course is full");
            return response;
        }
        Enrollment enrollment;
        enrollment.studentId = student->id;
        enrollment.courseId = course->id;
        enrollments = (Enrollment *)realloc(enrollments, (enrollments_size + 1) * sizeof(Enrollment));
        enrollments[enrollments_size++] = enrollment;
        course->enrolledStudents++;
        saveData();
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        sprintf(response, "Successfully enrolled in %s - %s", course->code, course->name);
    }
    else if (strcmp(command, "UNENROLL") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        if (!courseCode)
        {
            strcpy(response, "Invalid format");
            return response;
        }
        acquireWriteLock(COURSE_FILE);
        acquireWriteLock(ENROLLMENT_FILE);
        Course *course = findCourseByCode(courseCode);
        if (!course)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Course not found");
            return response;
        }
        int found = 0;
        for (int i = 0; i < enrollments_size; i++)
        {
            if (enrollments[i].studentId == student->id && enrollments[i].courseId == course->id)
            {
                for (int j = i; j < enrollments_size - 1; j++)
                {
                    enrollments[j] = enrollments[j + 1];
                }
                enrollments_size--;
                enrollments = (Enrollment *)realloc(enrollments, enrollments_size * sizeof(Enrollment));
                found = 1;
                break;
            }
        }
        if (!found)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(response, "Not enrolled in this course");
            return response;
        }
        course->enrolledStudents--;
        saveData();
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        sprintf(response, "Successfully unenrolled from %s - %s", course->code, course->name);
    }
    else if (strcmp(command, "VIEW_ENROLLED") == 0)
    {
        acquireReadLock(COURSE_FILE);
        acquireReadLock(ENROLLMENT_FILE);
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "Enrolled courses:\n");
        int hasEnrollments = 0;
        for (int i = 0; i < enrollments_size; i++)
        {
            if (enrollments[i].studentId == student->id)
            {
                Course *course = findCourseById(enrollments[i].courseId);
                if (course)
                {
                    char line[256];
                    sprintf(line, "Code: %s, Name: %s\n", course->code, course->name);
                    strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
                    hasEnrollments = 1;
                }
            }
        }
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        if (!hasEnrollments)
        {
            strcpy(response, "You are not enrolled in any courses");
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
        strcpy(result, "Available courses:\n");
        for (int i = 0; i < courses_size; i++)
        {
            User *faculty = findUserById(courses[i].facultyId);
            char line[256];
            sprintf(line, "Code: %s, Name: %s, Faculty: %s, Available seats: %d/%d\n",
                    courses[i].code, courses[i].name,
                    faculty ? faculty->username : "Unknown",
                    courses[i].totalSeats - courses[i].enrolledStudents,
                    courses[i].totalSeats);
            strncat(result, line, BUFFER_SIZE - strlen(result) - 1);
        }
        releaseLock(COURSE_FILE);
        strcpy(response, result);
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
        if (strcmp(student->password, oldPassword) != 0)
        {
            strcpy(response, "Incorrect current password");
            return response;
        }
        strncpy(student->password, newPassword, MAX_STR - 1);
        student->password[MAX_STR - 1] = '\0';
        saveData();
        strcpy(response, "Password changed successfully");
    }
    else
    {
        strcpy(response, "Invalid student command");
    }

    return response;
}
