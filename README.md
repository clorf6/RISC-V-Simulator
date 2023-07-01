# RISC-V-Simulator
Written by clorf/蒋松霖

##### 程序功能概述

用 C++ 实现一个简单的 RISC-V CPU 模拟器，能够支持并行运行。

##### 主题逻辑说明

采用 Tomasulo 算法，由于 C++ 是在用串行模拟并行，因此用乱序执行来说明并行。只要能够乱序执行各个元件，则说明各个元件是独立的，可以并行执行。

CPU 运行一个指令大致分为五个阶段：读取指令，解析指令，运算，访存，写回。在 Tomasulo 算法中，会将读取指令与解析指令放在一起，再将指令信息传给其他元件（例如 Reorder Buffer，Reservation Station 等），这个阶段称为**发射(Issue)**，然后再把指令从 Reservation Station 中取出放入 ALU 中进行运算，这个阶段称为**执行(Execute)**，执行完毕后再将结果返回 Reorder Buffer，写回寄存器或内存，即访存和写回，这个阶段称为**提交(Commit)**。乱序执行即要保证这些阶段在同一指令周期内可以以任意顺序执行。

Tomasulo 算法的架构如下（来自 CAAQA）：

![image-20230701152027452](https://clorf-imagehost.oss-cn-shanghai.aliyuncs.com/2023/07/image-20230701152027452-1e37eb.png)

##### 代码文件结构

```
RISC-V-S
├── CMakeLists.txt
├── README.md
└── src
    ├── Bus.cpp
    ├── Bus.h
    ├── CircularQueue.h
    ├── Exception.h
    ├── Instructions.cpp
    ├── Instructions.h
    ├── Memory.cpp
    ├── Memory.h
    ├── Predictor.cpp
    ├── Predictor.h
    ├── Register.cpp
    ├── Register.h
    ├── ReorderBuffer.cpp
    ├── ReorderBuffer.h
    ├── ReservationStation.cpp
    ├── ReservationStation.h
    ├── Unit.cpp
    ├── Unit.h
    ├── Utils.cpp
    ├── Utils.h
    └── main.cpp
```

##### 其他补充说明

将 Load 与 Store 相关指令也放入了 Reservation Station 中执行，即将 Load Store Buffer 的相关实现合并到 Reservation Station 中。

采用了混合分支预测，用二位饱和计数器作为选择器，对应局部预测和全局预测两个不同的选择。局部预测与全局预测均用两级自适应预测实现。
