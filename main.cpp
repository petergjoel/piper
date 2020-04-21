#include <iostream>
#include <stdlib.h>
#include "piper.h"

int main() {

    piper p;

    if(!p.run("/bin/python3", "python3", "test.py", nullptr))
    {
        p.send("hello python\n");
        p.readline(std::cout);
        std::cout << "DONE MAIN" << std::endl;
    }

    return 0;
}
