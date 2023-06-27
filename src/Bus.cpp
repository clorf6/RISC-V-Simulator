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
    int a[3] = {0, 1, 2};
    memory.ReadCode();
    while (flag) {
//        printf("-----\n");
//        printf("clock %d\n",clock);
        std::random_shuffle(a, a + 3);
        for (int i : a) {
            if (!i)  {
//                printf("Issue\n");
                Issue();
            }
            else if (i == 1) {
//                printf("Execute\n");
                Execute();
            }
            else {
//                printf("Commit\n");
                flag = Commit();
            }
        }
        //if (!flag) printf("%d\n",clock);
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
//            printf("%d\nnow %s %d %d pos %d\n", i, getEnumName(reservationStation.station[i].name),
//                   reservationStation.station[i].busy, reservationStation.station[i].ready, reservationStation.station[i].pos);
//            printf("nex %s %d %d pos %d\n",getEnumName(reservationStation.nex_station[i].name),
//                   reservationStation.nex_station[i].busy, reservationStation.nex_station[i].ready, reservationStation.station[i].pos);
//        }
//        printf("~ LSU ~\n");
//        printf("busy %d %d %d %d\n",reservationStation.LSU.busy, reservationStation.LSU.ready,
//               reservationStation.LSU.val, reservationStation.LSU.done);
//        printf("~ compALU ~\n");
//        for (int i = 0; i < 3; i++) {
//            printf("busy %d %d %d\n",reservationStation.compALU[i].busy,reservationStation.compALU[i].ready,reservationStation.compALU[i].val);
//        }
//        printf("~ AddALU ~\n");
//        for (int i = 0; i < 3; i++) {
//            printf("busy %d %d %d\n",reservationStation.addALU[i].busy,reservationStation.addALU[i].ready,reservationStation.addALU[i].val);
//        }
//        printf("-----\n");
        Flush();
    }
}

#endif //RISC_V_SIMULATOR_BUS_CPP
