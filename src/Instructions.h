//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_INSTRUCTIONS_H
#define RISC_V_SIMULATOR_INSTRUCTIONS_H

#include <cstdio>
#include <iostream>
#include "Register.h"
#include "Memory.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"
#include "Utils.h"
#include "Predictor.h"

class InstructionUnit {
    friend class ReservationStation;

private:
    Register PC;
    Predictor predictor;
    bool Stall;

    static void StationInit(const InstructionName &);

    static void StationInitRegister(Bus &, const DataUnit &, bool);

public:
    void Issue(Bus &);
};

static ReorderBufferData RobData;
static StationData RSData;

#endif //RISC_V_SIMULATOR_INSTRUCTIONS_H
