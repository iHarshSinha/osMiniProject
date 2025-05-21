
#include "serverDeclaration.h"

User* users = NULL;
int users_size = 0;
Course* courses = NULL;
int courses_size = 0;
Enrollment* enrollments = NULL;
int enrollments_size = 0;

const char* USER_FILE = "users.txt";
const char* COURSE_FILE = "courses.txt";
const char* ENROLLMENT_FILE = "enrollments.txt";
sem_t mutex;




int main()
{
    sem_init(&mutex, 0, 1);

    signal(SIGINT, signalHandler);

    loadData();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt(SO_REUSEPORT) failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in inAddr;
    inAddr.sin_family = AF_INET;
    inAddr.sin_addr.s_addr = INADDR_ANY;
    inAddr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&inAddr, sizeof(inAddr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Academia Portal Server started on port %d\n", PORT);
    printf("Waiting for connections...\n");

    while (1)
    {
        struct sockaddr_in clientAddr;
        socklen_t client_addrlen = sizeof(clientAddr);

        int *client_sock = (int *)malloc(sizeof(int));
        *client_sock = accept(server_fd, (struct sockaddr *)&clientAddr, &client_addrlen);

        if (*client_sock < 0)
        {
            perror("Accept failed");
            free(client_sock);
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("New connection from %s:%d\n", client_ip, ntohs(clientAddr.sin_port));

        pthread_t myThread;
        if (pthread_create(&myThread, NULL, handleClient, (void *)client_sock) != 0)
        {
            perror("Thread creation failed");
            close(*client_sock);
            free(client_sock);
            continue;
        }

        pthread_detach(myThread);
    }

    close(server_fd);
    sem_destroy(&mutex);
    free(users);
    free(courses);
    free(enrollments);

    return 0;
}


