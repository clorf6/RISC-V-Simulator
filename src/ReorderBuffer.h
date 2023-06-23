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

enum class CommitType {
    Register,
    Branch,
    Done
};

struct ReorderBufferData {
    CommitType type;
    DataUnit val;
    DataUnit pos;
    bool ready;
};

class ReorderBuffer {
private:
    CircularQueue<ReorderBufferData, 32> buffer;
    CircularQueue<ReorderBufferData, 32> nex_buffer;
public:
    ReorderBufferData& operator[](DataUnit);

    const ReorderBufferData& operator[](DataUnit) const;

    void Commit();

    void Add(const ReorderBufferData &);

    void Flush();

    bool full() const;

    bool empty() const;

    const ReorderBufferData& front() const;

    void pop();

    void clear();
};

#endif //RISC_V_SIMULATOR_REORDERBUFFER_H
