#include "serverDeclaration.h"

void loadData()
{
    FILE *file;
    char line[1024];

    file = fopen(USER_FILE, "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            char typeOfUser[20];
            int active;
            User user;
            

            sscanf(line, "%d %s %s %s %d", &user.id, user.username, user.password, typeOfUser, &active);

            if (strcmp(typeOfUser, "ADMIN") == 0)
                user.type = ADMIN;
            else if (strcmp(typeOfUser, "STUDENT") == 0)
                user.type = STUDENT;
            else if (strcmp(typeOfUser, "FACULTY") == 0)
                user.type = FACULTY;


            users = (User *)realloc(users, (users_size + 1) * sizeof(User));
            user.active = active;

            users[users_size++] = user;
        }
        fclose(file);
    }
    else
    {
        User admin;
        admin.id = 1;
        strcpy(admin.password, "admin123");
        strcpy(admin.username, "admin");
        admin.type = ADMIN;
        admin.active = 1;
        users = (User *)realloc(users, (users_size + 1) * sizeof(User));
        users[users_size++] = admin;

        file = fopen(USER_FILE, "w");
        fprintf(file, "%d %s %s ADMIN %d\n", admin.id, admin.username, admin.password, admin.active);
        fclose(file);
    }

    file = fopen(COURSE_FILE, "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            char newBuf[1024];
            Course course;
            
            if (sscanf(line, "%d %s %d %d %d %[^\n]", &course.id, course.code, &course.facultyId,
                       &course.totalSeats, &course.enrolledStudents, newBuf) >= 5)
            {
                strcpy(course.name, newBuf);
                courses = (Course *)realloc(courses, (courses_size + 1) * sizeof(Course));
                courses[courses_size++] = course;
            }
        }
        fclose(file);
    }

    file = fopen(ENROLLMENT_FILE, "r");
    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            Enrollment enrollment;
            sscanf(line, "%d %d", &enrollment.studentId, &enrollment.courseId);
            enrollments = (Enrollment *)realloc(enrollments, (enrollments_size + 1) * sizeof(Enrollment));
            enrollments[enrollments_size++] = enrollment;
        }
        fclose(file);
    }
}

void saveData()
{
    sem_wait(&mutex);

    FILE *uFile = fopen(USER_FILE, "w");
    for (int i = 0; i < users_size; i++)
    {
        const char *typeOfUser = users[i].type == ADMIN ? "ADMIN" : users[i].type == STUDENT ? "STUDENT"
                                                                                           : "FACULTY";
        fprintf(uFile, "%d %s %s %s %d\n", users[i].id, users[i].username,
                users[i].password, typeOfUser, users[i].active);
    }
    fclose(uFile);

    FILE *cFile = fopen(COURSE_FILE, "w");
    for (int i = 0; i < courses_size; i++)
    {
        fprintf(cFile, "%d %s %d %d %d %s\n", courses[i].id, courses[i].code,
                courses[i].facultyId, courses[i].totalSeats, courses[i].enrolledStudents,
                courses[i].name);
    }
    fclose(cFile);

    FILE *eFile = fopen(ENROLLMENT_FILE, "w");
    for (int i = 0; i < enrollments_size; i++)
    {
        fprintf(eFile, "%d %d\n", enrollments[i].studentId, enrollments[i].courseId);
    }
    fclose(eFile);

    sem_post(&mutex);
}
