//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_UTILS_H
#define RISC_V_SIMULATOR_UTILS_H

#include <cstdio>
#include <iostream>
#include "Exception.h"

#define STI(x) #x

using DataUnit = uint32_t;
using SignedDataUnit = int32_t;
using HalfDataUnit = uint16_t;
using SignedHalfDataUnit = int16_t;
using Byte = uint8_t;
using SignedByte = int8_t;

enum InstructionName {
    /*R type*/
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    /*I type*/
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    /*S types*/
    SB,
    SH,
    SW,
    /*B types*/
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    /*U types*/
    LUI,
    AUIPC,
    /*J types*/
    JAL,
    JALR,
    /*Others*/
    END
};

const char* getEnumName(InstructionName);

struct Instruction {
    InstructionName name;
    DataUnit rs1, rs2, rd;
    DataUnit imm;
};

Byte FetchOpcode(const DataUnit &);

DataUnit FetchRd(const DataUnit &);

Byte FetchFunct3(const DataUnit &);

DataUnit FetchRs1(const DataUnit &);

DataUnit FetchRs2(const DataUnit &);

DataUnit FetchFunct7(const DataUnit &);

DataUnit FetchIimm(const DataUnit &);

DataUnit FetchSimm(const DataUnit &);

DataUnit FetchBimm(const DataUnit &);

DataUnit FetchUimm(const DataUnit &);

DataUnit FetchJimm(const DataUnit &);

DataUnit FetchShamt(const DataUnit &);

DataUnit ISSignedExtend(const DataUnit &);

DataUnit BSignedExtend(const DataUnit &);

DataUnit JSignedExtend(const DataUnit &);

InstructionName FetchRInstruction(const DataUnit &);

InstructionName FetchIInstruction(const DataUnit &);

InstructionName FetchSInstruction(const DataUnit &);

InstructionName FetchBInstruction(const DataUnit &);

Instruction FetchInstruction(const DataUnit &);

#endif //RISC_V_SIMULATOR_UTILS_H
