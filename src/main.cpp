//
// Created by 屋顶上的小丑 on 2023/6/19.
//

#ifndef RISC_V_SIMULATOR_MAIN_CPP
#define RISC_V_SIMULATOR_MAIN_CPP

#include "Bus.h"
Bus bus;
int main() {
    //freopen("tak.data","r",stdin);
    //freopen("fuck","w",stdout);
    bus.Run();
    return 0;
}

#endif //RISC_V_SIMULATOR_MAIN_CPP