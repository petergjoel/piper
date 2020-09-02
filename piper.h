
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <signal.h>
#include <thread>
#include <chrono>
#include <sys/wait.h>

class piper {
protected:
    int _in[2];
    int _out[2];
    pid_t _pid;
public:
    piper()
    {
        if(pipe(_in) == -1)
        {
            exit(-1);
        }

        if(pipe(_out) == -1)
        {
            exit(-1);
        }
    }

    ~piper()
    {
    }

    bool run(const char* cmd, std::vector<const char*> args)
    {
        if(args.size() < 1) return false;
        args.push_back(nullptr);
        _pid = fork();
        if (_pid == (pid_t) 0)
        {
            close(_in[1]);
            close(_out[0]);
            dup2(_in[0], STDIN_FILENO);
            dup2(_out[1], STDOUT_FILENO);
            dup2(_out[1], STDERR_FILENO);
            execv(cmd, (char* const*)args.data());
            exit(0);
        }
        else if (_pid < (pid_t) 0)
        {
           std::cerr << "Fork failed" << std::endl;
        }
        else
        {
            close(_out[1]);
            close(_in[0]);
        }
        return false;
    }
 
    void block(size_t ms = 100) const {
        
        int status = 0;
        while (wait(&status) != _pid);
    }
    
    void block(std::ostream& ss, size_t ms = 100)
    {
        block(ms);
        const auto MX = 1024;
        char c[MX+1];
        do {
            auto r = read(_out[0], c, 1024);
            if(r <= 0) return;
            c[r] = '\0';
            ss << c;
            if(r < MX) return;
        } while(true);
    }
    
    void send(const char* cmd)
    {
        write(_in[1], cmd, strlen(cmd));
        fsync(_in[1]);
    }

    bool readchar(std::ostream& ss)
    {
        char c;
        int r = read(_out[0], &c, 1);
        if(!r){
            return false;
        }
        else if(c != '\n'){
            ss << c;
            return true;
        }
        else {
            c = 0;
            return false;
        }
    }

    void readline(std::ostream& ss)
    {
        while(true){
            if(!readchar(ss)) break;
        }
        ss << "\n";
    }

    void readchunk(std::ostream& ss, size_t timeout = 1000)
    {
        timeval tv;
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(_out[0], &rfds);

        while(true){
            while(true){
                tv.tv_sec = timeout / 1000;
                tv.tv_usec = timeout % 1000;

                int dataflag = select(_out[0] + 1, &rfds, NULL, NULL, &tv);
                if(dataflag && dataflag != -1) {
                    if(!readchar(ss)) break;
                }
                else
                {
                    return;
                }
            }
            ss << "\n";
        }
    }
};
