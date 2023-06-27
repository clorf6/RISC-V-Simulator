//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_H
#define RISC_V_SIMULATOR_BUS_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Exception.h"
#include "Register.h"
#include "Memory.h"
#include "Instructions.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"

class Bus {
    friend class InstructionUnit;
    friend class ReorderBuffer;
private:
    class Memory memory;
    RegisterFile registerFile;
    InstructionUnit instructionUnit;
    ReorderBuffer reorderBuffer;
    ReservationStation reservationStation;
    SignedDataUnit clock;
public:
    Bus();

    void Flush();

    void Issue();

    void Execute();

    bool Commit();

    void Run();
};

void Run(Bus &);

#endif //RISC_V_SIMULATOR_BUS_H
