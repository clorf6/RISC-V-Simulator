//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_INSTRUCTIONS_H
#define RISC_V_SIMULATOR_INSTRUCTIONS_H

#include <cstdio>
#include <iostream>
#include "Register.h"
#include "Memory.h"
#include "Utils.h"

enum class InstructionName {
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
    AUIPE,
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

#endif //RISC_V_SIMULATOR_INSTRUCTIONS_H
