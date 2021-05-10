//
// Created by 王智杰 on 2020/11/26.
//
#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main (int argc, char **argv) {
    std::vector<pid_t> kids;
    int pipe_gen_proj[2];
    pipe(pipe_gen_proj);

    auto child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_gen_proj[0], STDIN_FILENO);
        close(pipe_gen_proj[0]);
        close(pipe_gen_proj[1]);
        int proj = execv("ece650-proj_metric", argv);
        return proj;
    }
    else if (child_pid < 0) {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) {
        dup2(pipe_gen_proj[1], STDOUT_FILENO);
        close(pipe_gen_proj[0]);
        close(pipe_gen_proj[1]);
        int rgen = execv("/home/agurfink/ece650/graphGen/graphGen", argv);
        return rgen;
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

