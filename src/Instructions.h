//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_INSTRUCTIONS_H
#define RISC_V_SIMULATOR_INSTRUCTIONS_H

#include <cstdio>
#include <iostream>
#include "Register.h"
#include "Memory.h"
#include "Utils.h"
#include "Predictor.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"

class InstructionUnit {
    friend class ReservationStation;
    friend class ReorderBuffer;
    friend class Bus;
private:
    Register PC;
    Predictor predictor;
    bool Stall;

    static void StationInit(const InstructionName &);

    static void StationInitRegister(ReorderBuffer *, RegisterFile *, const DataUnit &, bool);

public:
    void Issue(ReorderBuffer *, ReservationStation *,
               RegisterFile *, Memory *, const DataUnit &);
};

static ReorderBufferData RobData;
static StationData RSData;

#endif //RISC_V_SIMULATOR_INSTRUCTIONS_H
