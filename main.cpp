#include <iostream>
#include "piper.h"

int main() {
    piper p;

    if(!p.run("/bin/gdb", "gdb"))
    {
        p.readchunk(std::cout, 1000);

        std::cout << "SENDING " << std::endl;
        p.send("p 10+10\n");
        p.readline(std::cout);
        p.send("help\n");
        p.readchunk(std::cout, 1000);

        std::string tmp;
        std::cin >> tmp;

        p.send("quit\n");
        std::cout << "DONE MAIN" << std::endl;
    }

    return 0;
}