//
// Created by 屋顶上的小丑 on 2023/6/23.
//

#ifndef RISC_V_SIMULATOR_EXCEPTION_H
#define RISC_V_SIMULATOR_EXCEPTION_H

#include <iostream>
#include <string>

class Exception {
private:
    const std::string message;
public:
    Exception(const std::string &str) : message(str) {}

    const std::string what() { return message; }
};

#endif //RISC_V_SIMULATOR_EXCEPTION_H
