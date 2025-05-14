#ifndef SERVERDECLARATION_H
#define SERVERDECLARATION_H
#include "../myincludes.h"



// macros
#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define MAX_COURSE_SEATS 10
#define MAX_USERS 1000
#define MAX_COURSES 1000
#define MAX_ENROLLMENTS 10000
#define MAX_STR 256



// structs


// User types
enum UserType {
    ADMIN = 1,
    STUDENT = 2,
    FACULTY = 3
};

// Structure to hold user information
typedef struct {
    int id;
    char username[MAX_STR];
    char password[MAX_STR];
    enum UserType type;
    int active; // 1 for true, 0 for false
} User;

// Structure to hold course information
typedef struct {
    int id;
    char code[MAX_STR];
    char name[MAX_STR];
    int facultyId;
    int totalSeats;
    int enrolledStudents;
} Course;

// Structure to hold enrollment information
typedef struct {
    int studentId;
    int courseId;
} Enrollment;



// mutex

extern sem_t mutex;



// global variables
// Global variables for storing data


extern User* users;
extern int users_size;
extern Course* courses;
extern int courses_size;
extern Enrollment* enrollments;
extern int enrollments_size;


// File paths
extern const char* USER_FILE;
extern const char* COURSE_FILE;
extern const char* ENROLLMENT_FILE;


// Function declarations
char *handleAdminRequest(const char *request, int userId);
char *handleStudentRequest(const char *request, int userId);
char *handleFacultyRequest(const char *request, int userId);
void loadData();
void saveData();
void *handleClient(void *client_socket);
char *processRequest(const char *request, int clientSocket);
char *loginUser(const char *username, const char *password);

User *findUserById(int id);
User *findUserByUsername(const char *username);
Course *findCourseById(int id);
Course *findCourseByCode(const char *code);
int isEnrolled(int studentId, int courseId);
void acquireReadLock(const char *filename);
void acquireWriteLock(const char *filename);
void releaseLock(const char *filename);
char *strdup(const char *s); // For systems lacking strdup





#endif