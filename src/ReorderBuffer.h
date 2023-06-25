//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_REORDERBUFFER_H
#define RISC_V_SIMULATOR_REORDERBUFFER_H

#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "Register.h"
#include "Memory.h"
#include "Instructions.h"
#include "ReservationStation.h"
#include "CircularQueue.h"

enum CommitType {
    RegisterWrite,
    MemoryWrite,
    Branch,
    Done
};

struct ReorderBufferData {
    InstructionName name;
    CommitType type;
    DataUnit val;
    DataUnit pos, add;
    bool ready, PredictedAns;
};

class ReorderBuffer {
private:
    CircularQueue<ReorderBufferData, 32> buffer;
    CircularQueue<ReorderBufferData, 32> nex_buffer;
public:
    ReorderBufferData& operator[](DataUnit);

    const ReorderBufferData& operator[](DataUnit) const;

    bool Commit(InstructionUnit &, ReservationStation &,
                RegisterFile &, Memory &);

    DataUnit Add(const ReorderBufferData &, RegisterFile &);

    void Flush();

    bool full() const;

    bool empty() const;

    const ReorderBufferData& front() const;

    DataUnit Head() const;

    void pop();

    void clear();
};

#endif //RISC_V_SIMULATOR_REORDERBUFFER_H
