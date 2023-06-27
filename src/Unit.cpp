//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_UNIT_CPP
#define RISC_V_SIMULATOR_UNIT_CPP

#include "Unit.h"
#include "ReservationStation.h"
#include "ReorderBuffer.h"

void Unit::clear() {
    busy = 0;
    ready = false;
}

void AR::clear() {
    Unit::clear();
    done = false;
}

void Unit::Flush() {
    if (ready && (!busy)) {
        ready = false;
    }
    if (busy) {
        --busy;
        if (!busy) ready = true;
    }
}

void Unit::Return(ReorderBuffer *reorderBuffer,
                  ReservationStation *reservationStation) {
    if (ready) {
        //printf("Ret %d %d %d %s\n", pos, val, (*reservationStation)[pos].pos,
        //       getEnumName((*reservationStation)[pos].name));
        (*reservationStation)[pos].busy =
        (*reservationStation)[pos].ready = false;
        (*reorderBuffer)[(*reservationStation)[pos].pos].ready = true;
        (*reorderBuffer)[(*reservationStation)[pos].pos].val = val;
    }
}

void AR::Return(ReorderBuffer *reorderBuffer,
                ReservationStation *reservationStation) {
    if (ready) {
        //printf("Retadd %d\n", add);
        (*reorderBuffer)[(*reservationStation)[pos].pos].add = add;
    }
    Unit::Return(reorderBuffer, reservationStation);
}

void AddALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch (Name) {
        case InstructionName::ADD:
        case InstructionName::ADDI: {
            val = rs1 + rs2;
            break;
        }
        case InstructionName::SUB: {
            val = rs1 - rs2;
            break;
        }
        default:
            throw Exception("Wrong Instruction in Unit");
    }
}

void ShiftALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                       const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch (Name) {
        case InstructionName::SLL:
        case InstructionName::SLLI: {
            val = rs1 << rs2;
            break;
        }
        case InstructionName::SRL:
        case InstructionName::SRLI: {
            val = rs1 >> rs2;
            break;
        }
        case InstructionName::SRA:
        case InstructionName::SRAI: {
            val = static_cast<SignedDataUnit>(rs1) >> rs2;
            break;
        }
        default:
            throw Exception("Wrong Instruction in Unit");
    }
}

void BitALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch (Name) {
        case InstructionName::OR:
        case InstructionName::ORI: {
            val = rs1 | rs2;
            break;
        }
        case InstructionName::AND:
        case InstructionName::ANDI: {
            val = rs1 & rs2;
            break;
        }
        case InstructionName::XOR:
        case InstructionName::XORI: {
            val = rs1 ^ rs2;
            break;
        }
        default:
            throw Exception("Wrong Instruction in Unit");
    }
}

void CompALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                      const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch (Name) {
        case InstructionName::SLT:
        case InstructionName::SLTI:
        case InstructionName::BLT: {
            val = static_cast<DataUnit>(
                    static_cast<SignedDataUnit>(rs1)
                    < static_cast<SignedDataUnit>(rs2));
            break;
        }
        case InstructionName::SLTU:
        case InstructionName::SLTIU:
        case InstructionName::BLTU: {
            val = static_cast<DataUnit>(rs1 < rs2);
            break;
        }
        case InstructionName::BEQ: {
            val = static_cast<DataUnit>(rs1 == rs2);
            break;
        }
        case InstructionName::BNE: {
            val = static_cast<DataUnit>(rs1 != rs2);
            break;
        }
        case InstructionName::BGE: {
            val = static_cast<DataUnit>(
                    static_cast<SignedDataUnit>(rs1)
                    >= static_cast<SignedDataUnit>(rs2));
            break;
        }
        case InstructionName::BGEU: {
            val = static_cast<DataUnit>(rs1 >= rs2);
            break;
        }
        default:
            throw Exception("Wrong Instruction in Unit");
    }
}

void AR::Execute(const InstructionName &Name, const DataUnit &Pos,
                 const DataUnit &rs, const DataUnit &Add, const DataUnit &tim,
                 Memory *memory) {
    busy = tim, pos = Pos;
    ready = false;
    done = true;
    switch (Name) {
        case InstructionName::LB: {
            val = static_cast<DataUnit>(memory->ReadSignedByte(Add));
            break;
        }
        case InstructionName::LH: {
            val = static_cast<DataUnit>(memory->ReadSignHalfDataUnit(Add));
            break;
        }
        case InstructionName::LW: {
            val = static_cast<DataUnit>(memory->ReadSignedDataUnit(Add));
            break;
        }
        case InstructionName::LBU: {
            val = static_cast<DataUnit>(memory->ReadByte(Add));
            break;
        }
        case InstructionName::LHU: {
            val = static_cast<DataUnit>(memory->ReadHalfDataUnit(Add));
            break;
        }
        case InstructionName::SB:
        case InstructionName::SH:
        case InstructionName::SW: {
            val = rs;
            add = Add;
            break;
        }
        default:
            throw Exception("Wrong Instruction in Unit");
    }
}

#endif //RISC_V_SIMULATOR_UNIT_CPP