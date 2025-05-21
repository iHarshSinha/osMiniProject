#include "serverDeclaration.h"
char *handleStudentRequest(const char *request, int userId)
{
    char *res = (char *)malloc(BUFFER_SIZE);
    User *student = findUserById(userId);
    if (!student || student->type != STUDENT)
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

    if (strcmp(inp, "ENROLL") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        if (!courseCode)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        acquireWriteLock(COURSE_FILE);
        acquireWriteLock(ENROLLMENT_FILE);
        Course *course = findCourseByCode(courseCode);
        if (!course)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(res, "Course not found in the database");
            return res;
        }
        if (isEnrolled(student->id, course->id))
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(res, "you are already enrolled in this course");
            return res;
        }
        if (course->enrolledStudents >= course->totalSeats)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(res, "No seat is empty in the course");
            return res;
        }
        Enrollment e;
        e.studentId = student->id;
        e.courseId = course->id;
        enrollments = (Enrollment *)realloc(enrollments, (enrollments_size + 1) * sizeof(Enrollment));
        enrollments[enrollments_size++] = e;
        course->enrolledStudents++;
        saveData();
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        sprintf(res, "Successfully enrolled in %s - %s", course->code, course->name);
    }
    else if (strcmp(inp, "UNENROLL") == 0)
    {
        char *courseCode = strtok(NULL, " ");
        if (!courseCode)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        acquireWriteLock(COURSE_FILE);
        acquireWriteLock(ENROLLMENT_FILE);
        Course *course = findCourseByCode(courseCode);
        if (!course)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(res, "Course not found");
            return res;
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
                enrollments_size-=1;
                enrollments = (Enrollment *)realloc(enrollments, enrollments_size * sizeof(Enrollment));
                found = 1;
                break;
            }
        }
        if (!found)
        {
            releaseLock(COURSE_FILE);
            releaseLock(ENROLLMENT_FILE);
            strcpy(res, "Not enrolled in this course");
            return res;
        }
        course->enrolledStudents--;
        saveData();
        releaseLock(COURSE_FILE);
        releaseLock(ENROLLMENT_FILE);
        sprintf(res, "Successfully deenrolled from %s - %s", course->code, course->name);
    }
    else if (strcmp(inp, "VIEW_ENROLLED") == 0)
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
            strcpy(res, "You are not enrolled in any courses");
        }
        else
        {
            strcpy(res, result);
        }
        free(result);
    }
    else if (strcmp(inp, "VIEW_COURSES") == 0)
    {
        acquireReadLock(COURSE_FILE);
        char *result = (char *)malloc(BUFFER_SIZE);
        strcpy(result, "list of available courses:\n");
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
        strcpy(res, result);
        free(result);
    }
    else if (strcmp(inp, "CHANGE_PASSWORD") == 0)
    {
        char *oldPassword = strtok(NULL, " ");
        char *newPassword = strtok(NULL, " ");
        if (!oldPassword || !newPassword)
        {
            strcpy(res, "Invalid format");
            return res;
        }
        if (strcmp(student->password, oldPassword) != 0)
        {
            strcpy(res, "Incorrect current password");
            return res;
        }
        strncpy(student->password, newPassword, MAX_STR - 1);
        student->password[MAX_STR - 1] = '\0';
        saveData();
        strcpy(res, "Password changed successfully");
    }
    else
    {
        strcpy(res, "Invalid student inp");
    }

    return res;
}
