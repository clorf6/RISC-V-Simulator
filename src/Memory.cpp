//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_MEMORY_CPP
#define RISC_V_SIMULATOR_MEMORY_CPP

#include "Memory.h"

Memory::Memory(DataUnit size) {
    head = new Byte[size];
    ReadCode();
}

DataUnit Memory::ReadDataUnit(DataUnit pos) const {
    return *(reinterpret_cast<DataUnit*>(head + pos));
}

SignedDataUnit Memory::ReadSignedDataUnit(DataUnit pos) const {
    return *(reinterpret_cast<SignedDataUnit*>(head + pos));
}

HalfDataUnit Memory::ReadHalfDataUnit(DataUnit pos) const {
    return *(reinterpret_cast<HalfDataUnit*>(head + pos));
}

SignedHalfDataUnit Memory::ReadSignHalfDataUnit(DataUnit pos) const {
    return *(reinterpret_cast<SignedHalfDataUnit*>(head + pos));
}

Byte Memory::ReadByte(DataUnit pos) const {
    return *(head + pos);
}

SignedByte Memory::ReadSignedByte(DataUnit pos) const {
    return *(reinterpret_cast<SignedByte*>(head + pos));
}

void Memory::WriteDataUnit(DataUnit pos, DataUnit val) {
    *(reinterpret_cast<DataUnit*>(head + pos)) = val;
}

void Memory::WriteHalfDataUnit(DataUnit pos, HalfDataUnit val) {
    *(reinterpret_cast<HalfDataUnit*>(head + pos)) = val;
}

void Memory::WriteByte(DataUnit pos, Byte val) {
    *(head + pos) = val;
}

void Memory::ReadCode() {
    std::string code;
    DataUnit now = 0;
    while (std::cin >> code) {
        if (code[0] == '@') {
            now = std::stoi(code.substr(1), nullptr, 16);
        } else {
            head[now] = static_cast<Byte>(std::stoi(code, nullptr, 16));
            now++;
        }
    }
}

Memory::~Memory() {
    delete[] head;
    head = nullptr;
}

#endif //RISC_V_SIMULATOR_MEMORY_CPP
