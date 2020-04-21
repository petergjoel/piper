#include <iostream>
#include <stdlib.h>
#include "piper.h"

int main() {

    chdir("/home/petko/Devel/wcet_uppaal/stepSimulator");
    piper p;

    if(!p.run("/bin/scala", "scala", "/home/petko/Devel/wcet_uppaal/stepSimulator/stepSimulator.scala", "r0=0"))
    {
        p.readline(std::cout);
        std::cout << "DONE MAIN" << std::endl;
    }

    return 0;
}
