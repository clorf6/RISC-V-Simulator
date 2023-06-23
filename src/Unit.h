//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_UNIT_H
#define RISC_V_SIMULATOR_UNIT_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Instructions.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"

class Unit {
    friend class ReservationStation;
protected:
    bool ready;
    DataUnit busy;
    DataUnit val;
    DataUnit pos;
public:
    Unit() : ready(false), busy(0), val(0), pos(0) {}

    void clear();

    void Flush();

    void Return(ReorderBuffer &, ReservationStation &);

    virtual void Execute(const InstructionName &, const DataUnit &,
                         const DataUnit &, const DataUnit &, const DataUnit &);
};

class AddALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class ShiftALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class BitALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class CompALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &) override;
};

class MU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &,
                 Memory &);
};

#endif //RISC_V_SIMULATOR_UNIT_H
