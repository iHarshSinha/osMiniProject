
#include "clientDeclaration.h"


int client_fd = -1;
bool is_logged_in = false;
char current_user_type[20] = "";
int current_user_id = -1;






int main() {
   
    signal(SIGINT, cleanExit);
    
    connectToServer();
    
    while (true) {
        loginMenu();
    }
    
    return 0;
}