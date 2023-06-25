//
// Created by 屋顶上的小丑 on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H

#include <cstdio>
#include <iostream>
#include <string>
#include "Exception.h"
#include "Utils.h"

class Memory {
private:
    Byte *head;
public:
    Memory(DataUnit size);

    DataUnit ReadDataUnit(DataUnit) const;

    SignedDataUnit ReadSignedDataUnit(DataUnit) const;

    HalfDataUnit ReadHalfDataUnit(DataUnit) const;

    SignedHalfDataUnit ReadSignHalfDataUnit(DataUnit) const;

    Byte ReadByte(DataUnit) const;

    SignedByte ReadSignedByte(DataUnit) const;

    void WriteDataUnit(DataUnit, DataUnit);

    void WriteHalfDataUnit(DataUnit, HalfDataUnit);

    void WriteByte(DataUnit, Byte);

    void Write(DataUnit, DataUnit, const InstructionName &);

    void ReadCode();

    ~Memory();
};

#endif //RISC_V_SIMULATOR_MEMORY_H
