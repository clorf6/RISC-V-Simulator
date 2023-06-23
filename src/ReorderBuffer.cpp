//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_REORDERBUFFER_CPP
#define RISC_V_SIMULATOR_REORDERBUFFER_CPP

#include "ReorderBuffer.h"

ReorderBufferData &ReorderBuffer::operator[](DataUnit pos) {
    return nex_buffer[pos];
}

const ReorderBufferData &ReorderBuffer::operator[](DataUnit pos) const {
    return buffer[pos];
}

void ReorderBuffer::Add(const ReorderBufferData &now) {
    nex_buffer.push(now);
    /*TO DO,更新寄存器依赖*/
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
