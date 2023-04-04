#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

void printError(){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void runBasicCmd(char * argv[], const char* path)
{
    int ret = execv(path, argv);//Note: argv must be end with NULL
    if(ret == -1){
        printError();
    }
    exit(0);
}

void runCd(char* argv[], int size, int startIndex){
    if(size == 2){
        if(chdir(argv[startIndex+1]) != 0){
            printError();
        }
    }
    else{
        printError();
    }
}

void runPwd(char* argv[], int size, int redirect, int startIndex){
    if(size != 1 && !redirect){
        printError();
        return;
    }
    char pwd[1000];
    if(getcwd(pwd, 1000) == NULL){
        printError();
        return;
    }
    strcat(pwd, "\n\0");
    write(STDOUT_FILENO, pwd, strlen(pwd));
}

int isPipe(char* cmd){
    char* pipe = strchr(cmd, '|');
    if(pipe != NULL){
        return 1;
    }
    return 0;
}

int isMultiCmd(char* cmd){
    char* semi = strchr(cmd, ';');
    if(semi != NULL){
        return 1;
    }
    return 0;
}

void runOneCommand(char* args[], int size, int redirectFlag)
{
    // copy args into argv
    if(size == 0){
        return;
    }
    char* argv[size + 1];
    int i = 0;
    for(i = 0; i < size; i++){
        argv[i] = args[i];
    }
    argv[size] = NULL;

    if(strcmp(args[0], "loop") == 0){
        int loopTime = atoi(argv[1]);
        for (int l = 0; l<loopTime; l++){
            int d = dup(fileno(stdout));

            if(redirectFlag){
                // loop & redirection
                int fd;
                if((fd = open(argv[size - 1], O_CREAT | O_TRUNC | O_WRONLY, 0664)) < 0){
                    printError();
                    return;
                }
                dup2(fd,1);
                argv[i - 1] = NULL;

                if(strcmp(args[2], "cd") == 0){
                    runCd(argv, size-2, 2);
                }
                else if(strcmp(args[2], "pwd") == 0){
                    runPwd(argv, size-2, redirectFlag, 2);
                }
                else{
                    int pid = fork();
                    if(pid == 0){
                        runBasicCmd(argv+2, args[2]);
                    }
                    else if(pid > 0){
                        wait(NULL);
                    }
                    else{
                        printError();
                        exit(0);
                    }
                }
                fflush(stdout);
                //restore stdout
                dup2(d, 1);
                close(fd);
                close(d);
                clearerr(stdout);
             }
             else{
                // loop
                if(strcmp(args[2], "cd") == 0){
                    runCd(argv, size-2, 2);
                }
                else if(strcmp(args[2], "pwd") == 0){
                    runPwd(argv, size-2, redirectFlag, 2);
                }
                else{
                    int pid = fork();
                    if(pid == 0){
                        runBasicCmd(argv+2, args[2]);
                    }
                    else if(pid > 0){
                        wait(NULL);
                    }
                    else{
                        printError();
                        exit(0);
                    }
                }
             }
        }
    }
    else{
        // no loop
        int d = dup(fileno(stdout));

        if(redirectFlag){
            int fd;
            if((fd = open(argv[i - 1], O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0){
                printError();
                return;
            }
            dup2(fd,1);
            argv[i - 1] = NULL;
        }
        if(strcmp(argv[0], "exit") == 0){
            if(size == 1){
                exit(0);
            }
            else{
                printError();
            }
        }
        else if(strcmp(argv[0], "cd") == 0){
            runCd(argv, size, 0);
        }
        else if (strcmp(argv[0], "pwd") == 0){
            runPwd(argv, size, redirectFlag, 0);
        }
        else{
            int pid = fork();
            if(pid == 0){
                runBasicCmd(argv, argv[0]);
            }
            else if(pid > 0){
                wait(NULL);
            }
            else{
                printError();
                exit(0);
            }
        }
        if(redirectFlag){
            fflush(stdout);
            //restore stdout
            dup2(d, 1);
            close(d);
            clearerr(stdout);
        }
        }
    }

void runOneCommandWithPipe(char* cmd1args[], char* cmd2args[], int cmd1size, int cmd2size, int redirectFlag)
{
    // copy args into argv
    if(cmd1size == 0 | cmd2size == 0){
        printError();
        return;
    }
    char* cmd1argv[cmd1size + 1];
    int i = 0;
    for(i = 0; i < cmd1size; i++){
        cmd1argv[i] = cmd1args[i];
    }
    cmd1argv[cmd1size] = NULL;

    char* cmd2argv[cmd2size + 1];
    for(i = 0; i < cmd2size; i++){
        cmd2argv[i] = cmd2args[i];
    }
    cmd2argv[cmd2size] = NULL;

    if(strcmp(cmd1argv[0], "loop") == 0){
        int loopTime = atoi(cmd1argv[1]);
        for (int l = 0; l<loopTime; l++){
            int d = dup(fileno(stdout));
            if(redirectFlag){
                // loop & redirection & pipe
                int fd;
                if((fd = open(cmd2argv[cmd2size - 1], O_CREAT | O_TRUNC | O_WRONLY, 0664)) < 0){
                    printError();
                    return;
                }

                dup2(fd,1);
                cmd2argv[cmd2size - 1] = NULL;

                int pipefd[2];
                pipe(pipefd);
                int pid = fork();
                if(pid == 0){
                    dup2(pipefd[0],0);
                    close(pipefd[1]);
                    runBasicCmd(cmd2argv, cmd2argv[0]);
                }
                else if(pid > 0){
                    dup2(pipefd[1],1);
                    close(pipefd[0]);
                    runBasicCmd(cmd1argv+2, cmd1argv[2]);
                }
                else{
                    printError();
                    exit(0);
                }
                fflush(stdout);
                //restore stdout
                dup2(d, 1);
                close(fd);
                close(d);
                clearerr(stdout);
             }
             else{
                // loop & pipe
                int pipefd[2];
                pipe(pipefd);
                int pid = fork();
                if(pid == 0){
                    dup2(pipefd[0],0);
                    close(pipefd[1]);
                    runBasicCmd(cmd2argv, cmd2argv[0]);
                }
                else if(pid > 0){
                    dup2(pipefd[1],1);
                    close(pipefd[0]);
                    runBasicCmd(cmd1argv+2, cmd1argv[2]);
                }
                else{
                    printError();
                    exit(0);
                }
             }
        }
    }
    else{
        //pipe

        int pipefd[2];
        int pid1, pid2;

        pipe(pipefd);

        pid1 = fork();
        if(pid1 == 0){
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            execv(cmd1argv[0], cmd1argv);
            printError();
        }
        pid2 = fork();
        if(pid2 == 0){
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            execv(cmd2argv[0], cmd2argv);
            printError();
        }
        if(pid2 != 0){
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid2, NULL, 0);
        }
    }
}


void SimpleCaseRun(char* oneLine){
    // Deal with pipe and redirection
    int lineLen = strlen(oneLine);
    oneLine[lineLen-1] = '\0';
    // ***************************************************************
    // ********************** Redirection Check **********************
    // ***************************************************************
    int redirectFlag = 0;
    char* redirectsbl = strchr(oneLine, '>');
    if(redirectsbl != NULL){
        redirectFlag = 1;
        *(redirectsbl) = ' ';
        redirectsbl++;
        // check whether one sentence contain 2 redirectsbl
        char* r = strtok(redirectsbl, " ");// r is the first word after last space except a space
        if(r == NULL){
            // Error: Should be a filename after redirectory
            printError();
            return;
        }
        else{
            r = strtok(NULL, " ");// try to get another word. if really get one, that is illegal, report error
        }
        if(r != NULL){
            printError();
            return;
        }
    }
    else{
        redirectFlag = 0;
    }

    // ***************************************************************
    // ***************************  Pipe   ***************************
    // ***************************************************************
    int pipeFlag = 0;
    char* pipeSbl = strchr(oneLine, '|');
    if(pipeSbl != NULL){
        pipeFlag = 1;
    }

    if(pipeFlag == 1){
        // need to split in to two part: pipe before and pipe after. two args[]
        int cmd1size = 0;
        int cmd2size = 0;
        char *cmd1args[1000];
        char *cmd2args[1000];
        char *firstPhase = NULL;
        char *secondPhase = NULL;
        char *thirdPhase = NULL;
        char *fourthPhase = NULL;
        char *fifthPhase = NULL;
        char *oneLineCopy = malloc(1000*sizeof(char));
        strcpy(oneLineCopy, oneLine);

        if((cmd1args[cmd1size] = strtok_r(oneLine, "|", &firstPhase)) != NULL){
            oneLine = cmd1args[cmd1size];
            while((cmd1args[cmd1size] = strtok_r(oneLine, " ", &secondPhase)) != NULL){
                oneLine = cmd1args[cmd1size];
                    while((cmd1args[cmd1size] = strtok_r(oneLine, "\t", &thirdPhase)) != NULL){
                        cmd1size++;
                        oneLine = NULL;
                    }
                    oneLine = NULL;
                }
        }
        else{
            printError();
            return;
        }
        cmd2args[cmd2size] = strtok_r(oneLineCopy, "|", &fourthPhase);


        if((cmd2args[cmd2size] = strtok_r(NULL, "|", &fourthPhase)) != NULL){
            oneLineCopy = cmd2args[cmd2size];
            while((cmd2args[cmd2size] = strtok_r(oneLineCopy, " ", &fourthPhase)) != NULL){
                oneLineCopy = cmd2args[cmd2size];
                    while((cmd2args[cmd2size] = strtok_r(oneLineCopy, "\t", &fifthPhase)) != NULL){
                        cmd2size++;
                        oneLineCopy = NULL;
                    }
                    oneLineCopy = NULL;
                }
        }
        else{
            printError();
            return;
        }
        runOneCommandWithPipe(cmd1args,cmd2args, cmd1size, cmd2size, redirectFlag);
    }
    else{
        // phase when no pipe
        int size = 0;
        char *args[1000];
        char *firstPhase = NULL;
        char *secondPhase = NULL;

        while((args[size] = strtok_r(oneLine, " ", &firstPhase)) != NULL){
            oneLine = args[size];
            while((args[size] = strtok_r(oneLine, "\t", &secondPhase)) != NULL){
                size++;
                oneLine = NULL;
            }
            oneLine = NULL;
        }
        runOneCommand(args, size, redirectFlag);
    }
    return;
}


int main(int argc, char const *argv[]){
    if(argc != 1){
        printError();
        exit(1);
    }
    while(1){
        char* pmt = "smash> ";
        write(STDOUT_FILENO, pmt, strlen(pmt));
        size_t lineLen = 0;
        char *oneLine = malloc(1000*sizeof(char));
        ssize_t read;

        if((read = getline(&oneLine, &lineLen, stdin)) != -1){

            if(strcmp(oneLine, "\n") == 0){
                continue;
            }
            int multiCmdFlag = isMultiCmd(oneLine);
            if(multiCmdFlag){
                // First phase the whole line into small cmds
                int cmdNum = 0;
                char *cmds[1000];
                char *PhasebBySemi = NULL;
                char *buf;
                while((cmds[cmdNum] = strtok_r(oneLine, ";", &PhasebBySemi)) != NULL){
                    oneLine = cmds[cmdNum];
                    cmdNum++;
                    oneLine = NULL;
                }
                for(int c = 0; c < cmdNum; c++){
                    SimpleCaseRun(cmds[c]);
                }
            }
            else{
                SimpleCaseRun(oneLine);
            }
        }
        else{
            // Input NULL
            break;
        }
    }
    return 0;
}

