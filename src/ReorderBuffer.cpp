//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_REORDERBUFFER_CPP
#define RISC_V_SIMULATOR_REORDERBUFFER_CPP

#include "ReorderBuffer.h"
#include "Instructions.h"
#include "ReservationStation.h"

ReorderBufferData &ReorderBuffer::operator[](DataUnit Pos) {
    return nex_buffer[Pos];
}

const ReorderBufferData &ReorderBuffer::operator[](DataUnit Pos) const {
    return buffer[Pos];
}

DataUnit ReorderBuffer::Add(const ReorderBufferData &now,
                            RegisterFile *registerFile) {
    nex_buffer.push(now);
    if (now.type == CommitType::RegisterWrite && now.pos) {
        (*registerFile)[now.pos].SetDependency(nex_buffer.Tail());
    }
    return nex_buffer.Tail();
}

bool ReorderBuffer::Commit(InstructionUnit *instructionUnit,
                           ReservationStation *reservationStation,
                           RegisterFile *registerFile, Memory *memory) {
    if (empty()) return true;
    if (!front().ready) return true;
    reservationStation->Update(this);
    switch (front().type) {
        case CommitType::RegisterWrite: {
            registerFile->Write(front().pos,
                                front().val,
                                Head());
            break;
        }
        case CommitType::MemoryWrite: {
            memory->Write(front().add,
                          front().val,
                          front().name);
            break;
        }
        case CommitType::Branch: {
            bool ans = static_cast<bool>(front().val);
            total++;
            (instructionUnit->predictor).Update(front().add, ans);
            if (front().PredictedAns != ans) {
                clear();
                registerFile->ResetDependency();
                reservationStation->clear();
                instructionUnit->Stall = false;
                instructionUnit->PC = front().pos;
                return true;
            } else correct++;
            break;
        }
        case CommitType::Done: {
            std::cout << std::dec << (static_cast<HalfDataUnit>((*registerFile)[10]) & (0b1111'1111)) << std::endl;
            return false;
        }
        default:
            throw Exception("Commit Error");
    }
    switch (front().name) {
        case InstructionName::LB:
        case InstructionName::LH:
        case InstructionName::LW:
        case InstructionName::SB:
        case InstructionName::SH:
        case InstructionName::SW:
        case InstructionName::LBU:
        case InstructionName::LHU: {
            (reservationStation->LSU).done = false;
            break;
        }
        default:
            break;
    }
    pop();
    return true;
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

ReorderBufferData& ReorderBuffer::front() {
    return buffer.front();
}

DataUnit ReorderBuffer::Head() const {
    return buffer.Head();
}

void ReorderBuffer::pop() {
    nex_buffer.pop();
}

void ReorderBuffer::pop_back() {
    nex_buffer.pop_back();
}

void ReorderBuffer::clear() {
    nex_buffer.clear();
}



#endif //RISC_V_SIMULATOR_REORDERBUFFER_CPP