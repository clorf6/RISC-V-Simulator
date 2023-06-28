//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_CPP
#define RISC_V_SIMULATOR_BUS_CPP

#include "Bus.h"
#include <algorithm>

Bus::Bus() : memory(2048000), registerFile(),
             reorderBuffer(), reservationStation(), clock(0) {}

void Bus::Flush() {
    reorderBuffer.Flush();
    reservationStation.Flush();
    registerFile.Flush();
}

void Bus::Issue() {
    instructionUnit.Issue(&reorderBuffer, &reservationStation,
                          &registerFile, &memory, clock);
}

void Bus::Execute() {
    reservationStation.Execute(&memory, &instructionUnit);
}

bool Bus::Commit() {
    reservationStation.Return(&reorderBuffer);
    return reorderBuffer.Commit(&instructionUnit, &reservationStation,
                                &registerFile, &memory);
}

void Bus::Run() {
    bool flag = true;
    memory.ReadCode();
    while (flag) {
        Issue();
        Execute();
        flag = Commit();
        ++clock;
        Flush();
    }
}

#endif //RISC_V_SIMULATOR_BUS_CPP
