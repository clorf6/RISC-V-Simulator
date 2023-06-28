//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_RESERVATIONSTATION_H
#define RISC_V_SIMULATOR_RESERVATIONSTATION_H

#include <cstdio>
#include <iostream>
#include "Exception.h"
#include "Unit.h"

class InstructionUnit;

struct StationData {
    bool busy, ready;
    InstructionName name;
    DataUnit Vj, Vk, clock;
    SignedDataUnit Qj, Qk;
    DataUnit pos;

    StationData() : name(InstructionName::ADD), busy(false), ready(false),
    Vj(0), Vk(0), pos(0), Qj(-1), Qk(-1) {}
};

constexpr static int StationSize = 32;
constexpr static int ALUSize = 3;

class ReservationStation {
    friend class ReorderBuffer;
private:
    StationData station[StationSize];
    StationData nex_station[StationSize];

    AddALU addALU[ALUSize];
    ShiftALU shiftALU[ALUSize];
    BitALU bitALU[ALUSize];
    CompALU compALU[ALUSize];
    AR LSU;

public:
    StationData& operator[] (DataUnit);

    const StationData& operator[] (DataUnit) const;

    void Flush();

    void clear();

    bool Add(const StationData &);

    void Execute(Memory *, InstructionUnit *);//给ALU&MU运行

    void Return(ReorderBuffer *);//返回结果给RoB

    void Update(const ReorderBuffer *);//更新保留站中的Q
};

#endif //RISC_V_SIMULATOR_RESERVATIONSTATION_H
