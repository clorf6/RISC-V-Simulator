//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_REGISTER_CPP
#define RISC_V_SIMULATOR_REGISTER_CPP

#include "Register.h"

Register::operator DataUnit &() {
    return data;
}

Register &Register::operator=(DataUnit other) {
    data = other;
    return (*this);
}

Register &Register::operator+=(SignedDataUnit val) {
    data += val;
    return (*this);
}

void Register::ResetDependency() {
    dependency = -1;
}

void Register::SetDependency(DataUnit other_dependency) {
    dependency = other_dependency;
}

Register& RegisterFile::operator[](DataUnit pos) {
    return nex_registers[pos];
}

const Register& RegisterFile::operator[](DataUnit pos) const {
    return registers[pos];
}

Register RegisterFile::Read(DataUnit pos) const {
    return registers[pos];
}

void RegisterFile::Write(DataUnit pos, DataUnit other_data, DataUnit other_dependency) {
    if (pos) {
        nex_registers[pos] = other_data;
        nex_registers[pos].SetDependency(other_dependency);
    }
}

void RegisterFile::ResetDependency() {
    for (int i = 0; i < RegisterNum; i++) nex_registers[i].ResetDependency();
}

void RegisterFile::Flush() {
    for (int i = 0; i < RegisterNum; i++) registers[i] = nex_registers[i];
}

#endif //RISC_V_SIMULATOR_REGISTER_CPP
