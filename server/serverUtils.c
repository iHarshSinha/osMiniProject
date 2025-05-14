#include "serverDeclaration.h"


// Find a user by ID
User *findUserById(int id)
{
    for (int i = 0; i < users_size; i++)
    {
        if (users[i].id == id)
        {
            return &users[i];
        }
    }
    return NULL;
}

// Find a user by username
User *findUserByUsername(const char *username)
{
    for (int i = 0; i < users_size; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            return &users[i];
        }
    }
    return NULL;
}

// Find a course by ID
Course *findCourseById(int id)
{
    for (int i = 0; i < courses_size; i++)
    {
        if (courses[i].id == id)
        {
            return &courses[i];
        }
    }
    return NULL;
}

// Find a course by code
Course *findCourseByCode(const char *code)
{
    for (int i = 0; i < courses_size; i++)
    {
        if (strcmp(courses[i].code, code) == 0)
        {
            return &courses[i];
        }
    }
    return NULL;
}

// Check if a student is enrolled in a course
int isEnrolled(int studentId, int courseId)
{
    for (int i = 0; i < enrollments_size; i++)
    {
        if (enrollments[i].studentId == studentId && enrollments[i].courseId == courseId)
        {
            return 1;
        }
    }
    return 0;
}

// Acquire a read lock on a file
void acquireReadLock(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd != -1)
    {
        flock(fd, LOCK_SH);
        close(fd);
    }
}

// Acquire a write lock on a file
void acquireWriteLock(const char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd != -1)
    {
        flock(fd, LOCK_EX);
        close(fd);
    }
}

// Release a lock on a file
void releaseLock(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd != -1)
    {
        flock(fd, LOCK_UN);
        close(fd);
    }
}

// strdup implementation for systems that lack it
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