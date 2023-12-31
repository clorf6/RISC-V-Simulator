//
// Created by 屋顶上的小丑 on 2023/6/25.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H

#include <cstdio>
#include <iostream>
#include <bitset>
#include "Utils.h"

constexpr static DataUnit LocalHistoryNum = 4;
constexpr static DataUnit GlobalHistoryNum = 16;
constexpr static DataUnit BranchNum = 1 << 16;

class LocalPredictor {
private:
    Byte history[BranchNum]{0};
    std::bitset<1 << LocalHistoryNum> prediction[BranchNum];
public:
    bool predict(DataUnit) const;

    void Update(DataUnit, bool);
};

class GlobalPredictor {
private:
    Byte history{};
    std::bitset<1 << GlobalHistoryNum> prediction;
public:
    bool predict() const;

    void Update(bool);
};

class Predictor {
private:
    LocalPredictor localPredictor;
    GlobalPredictor globalPredictor;
    DataUnit count;
public:
    Predictor() : count(1) {}

    bool predict(DataUnit) const;

    void Update(DataUnit, bool);
};

#endif //RISC_V_SIMULATOR_PREDICTOR_H
