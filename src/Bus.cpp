//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_CPP
#define RISC_V_SIMULATOR_BUS_CPP

#include "Bus.h"

Bus::Bus() : memory(4096000), registerFile(),
             reorderBuffer(), reservationStation(), clock(0) {}

void Bus::clear() {
    registerFile.ResetDependency();
    reorderBuffer.clear();
    reservationStation.clear();
    instructionUnit.Stall = false;
}

void Bus::Flush() {
    reorderBuffer.Flush();
    reservationStation.Flush();
    registerFile.Flush();
}

void Bus::Issue() {
    instructionUnit.Issue(reorderBuffer, reservationStation,
                          registerFile, memory);
}

void Bus::Execute() {
    reservationStation.Execute(memory, instructionUnit);
    reservationStation.Return(reorderBuffer);
}

bool Bus::Commit() {
    reorderBuffer.Commit(instructionUnit, reservationStation,
                         registerFile, memory);
}

void Bus::Run() {
    bool flag = true;
    while (flag) {
        Issue();
        Execute();
        flag = Commit();
        ++clock;
        Flush();
    }
}

#endif //RISC_V_SIMULATOR_BUS_CPP
