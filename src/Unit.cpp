//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_UNIT_CPP
#define RISC_V_SIMULATOR_UNIT_CPP

#include "Unit.h"

void Unit::clear() {
    busy = 0;
    ready = false;
}

void Unit::Flush() {
    if (busy) {
        --busy;
        if (!busy) ready = true;
    }
}

void Unit::Return(ReorderBuffer &reorderBuffer,
                 ReservationStation &reservationStation) {
    if (ready) {
        reservationStation[pos].busy = false;
        reorderBuffer[reservationStation[pos].pos].ready = true;
        reorderBuffer[reservationStation[pos].pos].val = val;
    }
}

void AddALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch(Name) {
        case InstructionName::ADD:
        case InstructionName::ADDI:
            val = rs1 + rs2;
        case InstructionName::SUB:
            val = rs1 - rs2;
        default:
            throw Exception("Wrong Instruction");
    }
}

void ShiftALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch(Name) {
        case InstructionName::SLL:
        case InstructionName::SLLI:
            val = rs1 << rs2;
        case InstructionName::SRL:
        case InstructionName::SRLI:
            val = rs1 >> rs2;
        case InstructionName::SRA:
        case InstructionName::SRAI:
            val = static_cast<SignedDataUnit>(rs1) >> rs2;
        default:
            throw Exception("Wrong Instruction");
    }
}

void BitALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch(Name) {
        case InstructionName::OR:
        case InstructionName::ORI:
            val = rs1 | rs2;
        case InstructionName::AND:
        case InstructionName::ANDI:
            val = rs1 & rs2;
        case InstructionName::XOR:
        case InstructionName::XORI:
            val = rs1 ^ rs2;
        default:
            throw Exception("Wrong Instruction");
    }
}

void CompALU::Execute(const InstructionName &Name, const DataUnit &Pos,
                     const DataUnit &rs1, const DataUnit &rs2, const DataUnit &tim) {
    busy = tim, pos = Pos;
    ready = false;
    switch(Name) {
        case InstructionName::SLT:
        case InstructionName::SLTI:
        case InstructionName::BLT:
            val = static_cast<DataUnit>(
                  static_cast<SignedDataUnit>(rs1)
                  < static_cast<SignedDataUnit>(rs2));
        case InstructionName::SLTU:
        case InstructionName::SLTIU:
        case InstructionName::BLTU:
            val = static_cast<DataUnit>(rs1 < rs2);
        case InstructionName::BEQ:
            val = static_cast<DataUnit>(rs1 == rs2);
        case InstructionName::BNE:
            val = static_cast<DataUnit>(rs1 != rs2);
        case InstructionName::BGE:
            val = static_cast<DataUnit>(
                  static_cast<SignedDataUnit>(rs1)
                  >= static_cast<SignedDataUnit>(rs2));
        case InstructionName::BGEU:
            val = static_cast<DataUnit>(rs1 >= rs2);
        default:
            throw Exception("Wrong Instruction");
    }
}

void MU::Execute(const InstructionName &Name, const DataUnit &Pos,
                 const DataUnit &rs, const DataUnit &add, const DataUnit &tim,
                 Memory &memory) {
    busy = tim, pos = Pos;
    ready = false;
    switch(Name) {
        case InstructionName::LB:
            val = static_cast<DataUnit>(memory.ReadSignedByte(add));
        case InstructionName::LH:
            val = static_cast<DataUnit>(memory.ReadSignHalfDataUnit(add));
        case InstructionName::LW:
            val = static_cast<DataUnit>(memory.ReadSignedDataUnit(add));
        case InstructionName::LBU:
            val = static_cast<DataUnit>(memory.ReadByte(add));
        case InstructionName::LHU:
            val = static_cast<DataUnit>(memory.ReadHalfDataUnit(add));
        case InstructionName::SB:
            memory.WriteByte(add, static_cast<Byte>(rs));
        case InstructionName::SH:
            memory.WriteHalfDataUnit(add, static_cast<HalfDataUnit>(rs));
        case InstructionName::SW:
            memory.WriteDataUnit(add, rs);
        default:
            throw Exception("Wrong Instruction");
    }
}

#endif //RISC_V_SIMULATOR_ALU_CPP