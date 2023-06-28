//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_UNIT_H
#define RISC_V_SIMULATOR_UNIT_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Memory.h"

class ReorderBuffer;
class ReservationStation;

class Unit {
    friend class ReservationStation;
protected:
    bool ready;
    DataUnit busy;
    DataUnit val;
    DataUnit pos;
public:
    Unit() : ready(false), busy(0), val(0), pos(0) {}

    void Flush();

    virtual void clear();

    virtual void Return(ReorderBuffer *, ReservationStation *);
};

class AddALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &);
};

class ShiftALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &);
};

class BitALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &);
};

class CompALU : public Unit {
public:
    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &);
};

class AR : public Unit {
    friend class ReorderBuffer;
    friend class ReservationStation;
private:
    DataUnit add;
    bool done;
public:
    AR(): Unit(), add(0) {}

    void clear() override;

    void Return(ReorderBuffer *, ReservationStation *) override;

    void Execute(const InstructionName &, const DataUnit &,
                 const DataUnit &, const DataUnit &, const DataUnit &,
                 Memory *);
};

#endif //RISC_V_SIMULATOR_UNIT_H
