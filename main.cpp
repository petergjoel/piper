#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "piper.h"

int main() {

    {
        piper p;
        if (!p.run("/bin/python3",{"python3", "test.py", nullptr})) {
            p.send("hello python\n");
            p.readline(std::cout);
            std::cout << "DONE MAIN" << std::endl;
        }
    }
    {
        piper p;
        if (!p.run("/home/pgj/.local/bin/verifyta",{"verifyta", "-s", "./no_rate.xml", nullptr})) {
            std::stringstream ss;
            p.readline(ss);
            p.block(ss, 100);
            std::cerr << "DONE VERIFYTA" << std::endl;
            std::cerr << ss.str() << std::endl;
        } else std::cerr << "ERROR" << std::endl;
    }

    return 0;
}
