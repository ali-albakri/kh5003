#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>

using namespace std;

int main() {
    string cmd;

    while (true) {
        cout << "theshell>";
        getline(cin, cmd);

        if (cmd == "quit") {
            cout << "Exiting..." << endl;
            break;
        }

        pid_t pid = fork();

        if (pid < 0){
            perror("Forking Error");
            return 1;
        }

        if (pid == 0) {
            char ccmd[256];
            strncpy(ccmd, cmd.c_str(), sizeof(ccmd));
            ccmd[sizeof(ccmd)-1] = '\0';

            char *args[32];
            int x = 0;
            char *part = strtok(ccmd, " ");
            while (part != nullptr && x < 63) {
                args[x++] = part;
                part = strtok(nullptr, " ");
            }
            args[x] = nullptr;

            execvp(args[0], args);
            perror("Execvp Failed");
        } else {
            wait(NULL);
        }
    }
}
