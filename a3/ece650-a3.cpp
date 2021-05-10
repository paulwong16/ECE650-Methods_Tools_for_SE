#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main (int argc, char **argv) {
    std::vector<pid_t> kids;
    int pipe_rgen_a1[2], pipe_a1_a2[2], pipe_a2_a3[2];
    pipe(pipe_rgen_a1);
    pipe(pipe_a1_a2);
    pipe(pipe_a2_a3);

    auto child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_rgen_a1[0], STDIN_FILENO);
        close(pipe_rgen_a1[0]);
        close(pipe_rgen_a1[1]);
        dup2(pipe_a1_a2[1], STDOUT_FILENO);
        close(pipe_a1_a2[0]);
        close(pipe_a1_a2[1]);
        char *argv1[3];
        argv1[0] = (char *) "python3";
        argv1[1] = (char *) "ece650-a1.py";
        argv1[2] = nullptr;
        return execvp("python3", argv1);
    }
    else if (child_pid < 0) {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_rgen_a1[1], STDOUT_FILENO);
        close(pipe_rgen_a1[0]);
        close(pipe_rgen_a1[1]);
        int rgen = execv("rgen", argv);
        return rgen;
    }
    else if (child_pid < 0) {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_a1_a2[0], STDIN_FILENO);
        close(pipe_a1_a2[0]);
        close(pipe_a1_a2[1]);
        int a2 = execv("ece650-a2", argv);
        return a2;
    }
    else if (child_pid < 0) {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_a1_a2[1], STDOUT_FILENO);
        close(pipe_a1_a2[0]);
        close(pipe_a1_a2[1]);
        while (!std::cin.eof()) {
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                break;
            }
            std::cout << line << std::endl;
            std::cout.flush();
        }
        return 0;
    }
    else if (child_pid < 0) {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    int status;
    waitpid(-1,&status,0);
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }
    return 0;
}
