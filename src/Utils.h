//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_UTILS_H
#define RISC_V_SIMULATOR_UTILS_H

#include <cstdio>
#include <iostream>

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

struct Instruction {
    InstructionName name;
    DataUnit rs1, rs2, rd;
    DataUnit imm;
};

#endif //RISC_V_SIMULATOR_UTILS_H
