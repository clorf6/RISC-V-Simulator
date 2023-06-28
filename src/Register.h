//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_REGISTER_H
#define RISC_V_SIMULATOR_REGISTER_H

#include <cstdio>
#include <iostream>
#include "Utils.h"

class Register {
    friend class InstructionUnit;
    friend class RegisterFile;
private:
    DataUnit data = 0;
    SignedDataUnit dependency = -1;
public:
    Register() : data(0), dependency(-1) {};

    operator DataUnit &();

    Register &operator=(DataUnit);

    Register &operator+=(SignedDataUnit);

    void ResetDependency();

    void SetDependency(DataUnit);
};

constexpr static int RegisterNum = 32;

class RegisterFile {
//    friend class InstructionUnit;
private:
    Register registers[RegisterNum];
    Register nex_registers[RegisterNum];
public:
    Register &operator[](DataUnit);

    const Register &operator[](DataUnit) const;

    void Write(DataUnit, DataUnit, DataUnit);

    void ResetDependency();

    void Flush();
};

#endif //RISC_V_SIMULATOR_REGISTER_H
