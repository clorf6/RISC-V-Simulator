//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_REGISTER_H
#define RISC_V_SIMULATOR_REGISTER_H

#include <cstdio>
#include <iostream>
#include "Utils.h"

class Register {
private:
    DataUnit data = 0;
    SignedDataUnit dependency = -1;
public:
    Register() : data(0), dependency(-1) {};

    Register(const Register &) = default;

    Register(Register &&) = default;

    Register(DataUnit other) : data(other), dependency(-1) {};

    Register &operator=(const Register &) = default;

    Register &operator=(Register &&) = default;

    Register &operator+=(SignedDataUnit);

    void ResetDependency();

    void SetDependency(DataUnit);

    DataUnit Data() const;

    DataUnit Dependency() const;

    ~Register() = default;
};

constexpr static int RegisterNum = 32;

class RegisterFile {
private:
    Register registers[RegisterNum];
    Register nex_registers[RegisterNum];
public:
    RegisterFile() = default;

    RegisterFile(const RegisterFile &) = default;

    RegisterFile(RegisterFile &&) = default;

    RegisterFile& operator=(const RegisterFile &) = default;

    RegisterFile& operator=(RegisterFile &&) = default;

    Register Read(DataUnit) const;

    void Write(DataUnit, DataUnit, DataUnit);

    void ResetDependency();

    void Flush();

    ~RegisterFile() = default;
};

#endif //RISC_V_SIMULATOR_REGISTER_H
