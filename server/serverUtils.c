#include "serverDeclaration.h"


User *findUserById(int userId)
{
    for (int i = 0; i < users_size; i++)
    {
        if (users[i].id == userId)
        {
            return &users[i];
        }
    }
    return NULL;
}

User *findUserByUsername(const char *uname)
{
    for (int i = 0; i < users_size; i++)
    {
        if (strcmp(users[i].username, uname) == 0)
        {
            return &users[i];
        }
    }
    return NULL;
}

Course *findCourseById(int courseId)
{
    for (int i = 0; i < courses_size; i++)
    {
        if (courses[i].id == courseId)
        {
            return &courses[i];
        }
    }
    return NULL;
}

Course *findCourseByCode(const char *courseCode)
{
    for (int i = 0; i < courses_size; i++)
    {
        if (strcmp(courses[i].code, courseCode) == 0)
        {
            return &courses[i];
        }
    }
    return NULL;
}

int isEnrolled(int sid, int cid)
{
    for (int i = 0; i < enrollments_size; i++)
    {
        if (enrollments[i].studentId == sid && enrollments[i].courseId == cid)
        {
            return 1;
        }
    }
    return 0;
}

void acquireReadLock(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd != -1)
    {
        flock(fd, LOCK_SH);
        close(fd);
    }
}

void acquireWriteLock(const char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd != -1)
    {
        flock(fd, LOCK_EX);
        close(fd);
    }
}

void releaseLock(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd != -1)
    {
        flock(fd, LOCK_UN);
        close(fd);
    }
}

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *new = (char *)malloc(len);
    if (new)
    {
        memcpy(new, s, len);
    }
    return new;
}