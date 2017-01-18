
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>


class piper {
protected:
    int _in[2];
    int _out[2];
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

    bool run(const char* path, const char* cmd)
    {
        pid_t pid = fork();
        if (pid == (pid_t) 0)
        {
            close(_in[1]);
            close(_out[0]);
            dup2(_in[0], STDIN_FILENO);
            dup2(_out[1], STDOUT_FILENO);
            dup2(_out[1], STDERR_FILENO);
            execl(path, cmd, NULL);
            return true;

        }
        else if (pid < (pid_t) 0)
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

    void send(const char* cmd)
    {
        write(_in[1], cmd, strlen(cmd));
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