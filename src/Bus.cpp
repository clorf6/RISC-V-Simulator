//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_CPP
#define RISC_V_SIMULATOR_BUS_CPP

#include "Bus.h"

Bus::Bus() : memory(4096000), registerFile(),
             reorderBuffer(), reservationStation(), clock(0) {}

void Bus::clear() {
    reorderBuffer.clear();
    reservationStation.clear();
}

void Bus::Flush() {
    reorderBuffer.Flush();
    reservationStation.Flush();
    registerFile.Flush();
}

void Bus::Issue() {
    instructionUnit.Issue(*this);
}

void Bus::Execute() {
    reservationStation.Execute(memory, instructionUnit);
    reservationStation.Return(reorderBuffer);
}

bool Bus::Commit() {
    if (reorderBuffer.empty()) return true;
    if (!reorderBuffer.front().ready) return true;
    reservationStation.Update(reorderBuffer);
    switch (reorderBuffer.front().type) {
        case CommitType::Register:{

        }
        case CommitType::Memory: {

            break;
        }
        case CommitType::Branch: {

        }
        case CommitType::Done: {

            return false;
        }
        default:
            throw Exception("Wrong Instruction");
    }
    reorderBuffer.pop();
    return true;
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
