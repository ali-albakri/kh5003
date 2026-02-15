#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    string cmd;
    bool batch = false;

if (argc == 2) {
    freopen(argv[1], "r", stdin);
    batch = true;
}

    while (true) {
        char cwd[256];
        getcwd(cwd, sizeof(cwd));
        if (!batch) {
            cout << cwd << "> ";
        }


        if (!getline(cin, cmd)) {
            break;
        }


        char ccmd[256];
        strncpy(ccmd, cmd.c_str(), sizeof(ccmd));
        ccmd[sizeof(ccmd)-1] = '\0';

        if (cmd.length() >= sizeof(ccmd)) {
            cout << "Command exceeds limit!" << endl;
            continue;
        }

        char *args[32];
        int x = 0;
        char *part = strtok(ccmd, " ");
        while (part != nullptr && x < 31) {
            args[x++] = part;
            part = strtok(nullptr, " ");
        }
        args[x] = nullptr;

        bool backgr = false;

        if (x > 0 && strcmp(args[x-1], "&") == 0) {
            backgr = true;
            args[x-1] = nullptr;
        }

        if (args[0] == nullptr) {
            continue;
        }

        // Internal Shell commands
        if (strcmp(args[0], "quit") == 0){
            if (!batch) {
                cout << "Exiting..." << endl;
            }
            break;
        }  else if (strcmp(args[0], "help") == 0) {
            system("more README.md");
            continue; }
            else if (strcmp(args[0], "cd") == 0) {
                if (args[1] == nullptr) {
                char cwd[256];
                getcwd(cwd, sizeof(cwd));
                cout << cwd << endl;
            } else {
                if (chdir(args[1]) != 0) {
                    perror("CD Error");
                } else {
                        char cwdnew[256];
                        getcwd(cwdnew, sizeof(cwdnew));
                        setenv("PWD", cwdnew, 1);
                }
            }
            continue;
        } else if (strcmp(args[0], "dir") == 0) {
            const char *pathdir;
            if (args[1] != nullptr) {
                pathdir = args[1];
            } else {
                pathdir = ".";
            }

            DIR *dir = opendir(pathdir);
            if (!dir) {
                perror("DIR Error");
                continue;
            }

            struct dirent *direntry;
            while ((direntry = readdir(dir)) != nullptr) {
                  cout << direntry->d_name << endl;
            }
            cout << endl;
            closedir(dir);
            continue;
        } else if (strcmp(args[0], "pause") == 0) {
            cout << "Press Enter to continue..." << flush;
            while (cin.get() != '\n'); 
            continue;
        } else if (strcmp(args[0], "echo") == 0) {
            for (int j = 1; args[j] != nullptr; j++) {
                cout << args[j];
                if (args[j+1] != nullptr) {
                     cout << " ";
                }
            }
            cout << endl;
            continue;
        } else if (strcmp(args[0], "environ") == 0) {
            extern char **environ;
            for (char **env = environ; *env != 0; env++) {
                cout << *env << endl;
            }
            continue;
        } else if (strcmp(args[0], "set") == 0) {
            if (args[1] && args[2]) {
                setenv(args[1], args[2], 1);
            } else {
                cout << "set (Variable) (Value)" << endl;
            }
            continue;
        } 

        pid_t pid = fork();

        if (pid < 0){
            perror("Forking Error");
            return 1;
        }
        
        if (pid == 0) {
            execvp(args[0], args);
            perror("Execvp Failed");
        } else {
            if (!backgr) {
                wait(NULL);
            }
        }
    }
    return 0;
}
