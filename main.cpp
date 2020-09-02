#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "piper.h"

int main() {

    {
        piper p;
        if (!p.run("/bin/python3",{"python3", "test.py"})) {
            p.send("hello python\n");
            p.readline(std::cout);
            std::cout << "DONE MAIN" << std::endl;
        }
    }
    {
        piper p;
        std::vector<const char*> args{"verifyta", "-s", "-f", "1000", "./no_rate.xml"};
        if (!p.run("/home/pgj/.local/bin/verifyta",args)) {
            std::stringstream ss;
            p.block(ss);
            std::cerr << "DONE VERIFYTA" << std::endl;
            std::cerr << ss.str() << std::endl;
        } else std::cerr << "ERROR" << std::endl;
    }

    return 0;
}
