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
#include "CircularQueue.h"
#include "Predictor.h"

class InstructionUnit;
class ReservationStation;

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
    bool ready;
    bool PredictedAns;
};

class ReorderBuffer {
private:
    CircularQueue<ReorderBufferData, 32> buffer;
    CircularQueue<ReorderBufferData, 32> nex_buffer;
public:
    int total, correct;

    ReorderBufferData& operator[](DataUnit);

    const ReorderBufferData& operator[](DataUnit) const;

    bool Commit(InstructionUnit *, ReservationStation *,
                RegisterFile *, Memory *);

    DataUnit Add(const ReorderBufferData &, RegisterFile *);

    void Flush();

    bool full() const;

    bool empty() const;

    ReorderBufferData& front();

    DataUnit Head() const;

    void pop();

    void pop_back();

    void clear();
};

#endif //RISC_V_SIMULATOR_REORDERBUFFER_H
