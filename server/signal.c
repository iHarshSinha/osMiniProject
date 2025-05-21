#include "serverDeclaration.h"

void signalHandler(int signal_num)
{
    printf("\nSignal with signal id: %d\n", signal_num);
    saveData();
    sem_destroy(&mutex);
    free(courses);
    free(enrollments);
    free(users);
    exit(signal_num);
}
