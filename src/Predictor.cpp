//
// Created by 屋顶上的小丑 on 2023/6/25.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_CPP
#define RISC_V_SIMULATOR_PREDICTOR_CPP

#include "Predictor.h"

bool LocalPredictor::predict(DataUnit add) const {
    return prediction[add][history[add]];
}

void LocalPredictor::Update(DataUnit add, bool ans) {
    prediction[add][history[add]] = ans;
    history[add] = (history[add] >> 1) | (ans ? 0b1000 : 0);
}

bool GlobalPredictor::predict() const {
    return prediction[history];
}

void GlobalPredictor::Update(bool ans) {
    prediction[history] = ans;
    history = (history >> 1) | (ans ? 0b1000'0000 : 0);
}

bool Predictor::predict(DataUnit add) const {
    if (count <= 1) return localPredictor.predict(add);
    else return globalPredictor.predict();
}

void Predictor::Update(DataUnit add, bool ans) {
    if (localPredictor.predict(add) == ans && count > 0) count--;
    if (globalPredictor.predict() == ans && count < 3) count++;
    localPredictor.Update(add, ans);
    globalPredictor.Update(ans);
}

#endif //RISC_V_SIMULATOR_PREDICTOR_CPP
