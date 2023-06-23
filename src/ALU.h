//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_ALU_H
#define RISC_V_SIMULATOR_ALU_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Instructions.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"

class ALU {
    friend class ReservationStation;
protected:
    bool ready;
    DataUnit busy;
    DataUnit val;
    DataUnit pos;
public:
    ALU() : ready(false), busy(0), val(0), pos(0) {}

    void clear();

    void Flush();

    void Return(ReorderBuffer &, ReservationStation &);

    virtual void Execute(const InstructionName &, const DataUnit &,
                         const DataUnit &, const DataUnit &, const DataUnit &);
};

class AddALU : public ALU {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class ShiftALU : public ALU {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class BitALU : public ALU {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class CompALU : public ALU {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

#endif //RISC_V_SIMULATOR_ALU_H
