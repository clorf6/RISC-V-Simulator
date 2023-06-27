//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_BUS_CPP
#define RISC_V_SIMULATOR_BUS_CPP

#include "Bus.h"

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
    reservationStation.Return(&reorderBuffer);
}

bool Bus::Commit() {
    return reorderBuffer.Commit(&instructionUnit, &reservationStation,
                         &registerFile, &memory);
}

void Bus::Run() {
    bool flag = true;
    memory.ReadCode();
    while (flag) {
        //printf("-----\n");
        //printf("clock %d\n",clock);
        Issue();
        Execute();
        flag = Commit();
        ++clock;
//        printf("~ register ~\n");
//        for (int i = 0; i < 32; i++)
//            printf("%d ",i);
//        printf("\n");
//        for (int i = 0; i < 32; i++)
//            printf("%d ",registerFile[i]);
//        printf("\n");
//        printf("~ reservation station ~\n");
//        for (int i = 0; i < 10; i++) {
//            printf("%d\nnow %s %d %d\n", i, getEnumName(reservationStation.station[i].name),
//                   reservationStation.station[i].busy, reservationStation.station[i].ready);
//            printf("nex %s %d %d\n",getEnumName(reservationStation.nex_station[i].name),
//                   reservationStation.nex_station[i].busy, reservationStation.nex_station[i].ready);
//        }
//        printf("~ ls buffer ~\n");
//        for (int i = reservationStation.LSstation.Head(), j = 8; i != reservationStation.LSstation.End() && j; i = reservationStation.LSstation.nex(i), j--) {
//            printf("%d\nls %s %d %d\n", i, getEnumName(reservationStation.LSstation[i].name),
//                   reservationStation.LSstation[i].busy, reservationStation.LSstation[i].ready);
//        }
//        printf("~ LSU ~\n");
//        printf("busy %d %d %d %d\n",reservationStation.LSU.busy, reservationStation.LSU.ready,
//               reservationStation.LSU.val, reservationStation.LSU.done);
//        printf("-----\n");
        Flush();
    }
}

#endif //RISC_V_SIMULATOR_BUS_CPP
