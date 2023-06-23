//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_H
#define RISC_V_SIMULATOR_BUS_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Exception.h"
#include "Instructions.h"
#include "ReservationStation.h"
#include "ReorderBuffer.h"
#include "Register.h"
#include "Memory.h"

class Bus {
private:
    Memory memory;
    RegisterFile registerFile;
    ReorderBuffer reorderBuffer;
    ReservationStation reservationStation;
    SignedDataUnit clock;
public:
    Bus();

    void clear();

    void Flush();

    void Issue();

    void Execute();

    bool Commit();

    void Run();
};

void Run(Bus &);

#endif //RISC_V_SIMULATOR_BUS_H
