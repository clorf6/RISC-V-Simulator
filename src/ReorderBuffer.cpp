//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_REORDERBUFFER_CPP
#define RISC_V_SIMULATOR_REORDERBUFFER_CPP

#include "ReorderBuffer.h"

ReorderBufferData &ReorderBuffer::operator[](DataUnit Pos) {
    return nex_buffer[Pos];
}

const ReorderBufferData &ReorderBuffer::operator[](DataUnit Pos) const {
    return buffer[Pos];
}

DataUnit ReorderBuffer::Add(const ReorderBufferData &now,
                            RegisterFile &registerFile) {
    nex_buffer.push(now);
    if (now.type == CommitType::Register && now.pos) {
        registerFile[now.pos].SetDependency(nex_buffer.Tail());
    }
    return nex_buffer.Tail();
}

void ReorderBuffer::Flush() {
    buffer = nex_buffer;
}

bool ReorderBuffer::full() const {
    return buffer.full();
}

bool ReorderBuffer::empty() const {
    return buffer.empty();
}

const ReorderBufferData& ReorderBuffer::front() const {
    return buffer.front();
}

void ReorderBuffer::pop() {
    nex_buffer.pop();
}

void ReorderBuffer::clear() {
    nex_buffer.clear();
}



#endif //RISC_V_SIMULATOR_REORDERBUFFER_CPP
