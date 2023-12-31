//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_CPP
#define RISC_V_SIMULATOR_BUS_CPP

#include "Bus.h"
#include <algorithm>
#include <random>

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
    int a[3] = {0, 1, 2};
    while (flag) {
        std::shuffle(a, a + 3, std::mt19937(std::random_device()()));
        for (int i : a) {
            if (!i) Issue();
            else if (i == 1) Execute();
            else flag = Commit();
        }
//        Issue();
//        Execute();
//        flag = Commit();
        ++clock;
        Flush();
    }
    if (!reorderBuffer.total) printf("No branch\n");
    else printf("%.6lf\n", 1.0 * reorderBuffer.correct / reorderBuffer.total);
}

#endif //RISC_V_SIMULATOR_BUS_CPP
