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

void ReorderBuffer::Commit() {
    if (buffer.empty()) return;
    if (!buffer.front().ready) return;
    switch (buffer.front().type) {
        case CommitType::Register: {

        }
        case CommitType::Memory: {

        }
        case CommitType::Branch: {

        }
        case CommitType::Done: {

        }
    }
    nex_buffer.pop();
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

void ReorderBuffer::clear() {
    nex_buffer.clear();
}



#endif //RISC_V_SIMULATOR_REORDERBUFFER_CPP
