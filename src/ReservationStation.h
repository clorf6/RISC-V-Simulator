//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_RESERVATIONSTATION_H
#define RISC_V_SIMULATOR_RESERVATIONSTATION_H

#include <cstdio>
#include <iostream>
#include "Exception.h"
#include "Instructions.h"
#include "ReorderBuffer.h"
#include "Bus.h"
#include "ALU.h"

struct StationData {
    bool busy, ready;
    InstructionName name;
    DataUnit Vj, Vk;
    SignedDataUnit Qj, Qk;
    DataUnit pos;

    StationData() : busy(false), ready(false), Qj(-1), Qk(-1) {}

    StationData(const InstructionName &Name, const DataUnit &vj,
                const DataUnit &vk, const DataUnit &Pos,
                const SignedDataUnit &qj = -1, const SignedDataUnit &qk = -1) :
                busy(false), ready(false), name(Name), Vj(vj), Vk(vk),
                Qj(qj), Qk(qk), pos(Pos) {}
};

constexpr static int StationSize = 32;
constexpr static int ALUSize = 3;

class ReservationStation {
private:
    StationData station[StationSize];
    StationData nex_station[StationSize];

    AddALU addALU[ALUSize];
    ShiftALU shiftALU[ALUSize];
    BitALU bitALU[ALUSize];
    CompALU compALU[ALUSize];
public:
    StationData& operator[] (DataUnit);

    const StationData& operator[] (DataUnit) const;

    void Flush();

    void clear();

    bool Add(const StationData &);

    void Execute();//给ALU运行

    void Return(ReorderBuffer &);//返回结果给RoB

    void Update(const ReorderBuffer &);//更新保留站中的Q
};

#endif //RISC_V_SIMULATOR_RESERVATIONSTATION_H
