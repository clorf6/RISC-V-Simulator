//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_RESERVATIONSTATION_CPP
#define RISC_V_SIMULATOR_RESERVATIONSTATION_CPP

#include "ReservationStation.h"
#include "Instructions.h"

StationData &ReservationStation::operator[](DataUnit pos) {
    return nex_station[pos];
}

const StationData &ReservationStation::operator[](DataUnit pos) const {
    return station[pos];
}

void ReservationStation::Flush() {
    for (int i = 0; i < StationSize; i++) station[i] = nex_station[i];
    LSU.Flush();
    for (int i = 0; i < ALUSize; i++) {
        addALU[i].Flush();
        shiftALU[i].Flush();
        bitALU[i].Flush();
        compALU[i].Flush();
    }
}

void ReservationStation::clear() {
    for (int i = 0; i < StationSize; i++) {
        nex_station[i].busy =
        nex_station[i].ready = false;
        station[i].busy =
        station[i].ready = false;
    }
    LSU.clear();
    for (int i = 0; i < ALUSize; i++) {
        addALU[i].clear();
        shiftALU[i].clear();
        bitALU[i].clear();
        compALU[i].clear();
    }
}

bool ReservationStation::Add(const StationData &now) {
    for (int i = 0; i < StationSize; i++) {
        if ((!station[i].busy) && (!station[i].ready)) {

            //printf("add %d %d %d %d %d\npos %d\n", i, now.Vj, now.Vk, now.Qj, now.Qk, now.pos);
            nex_station[i] = now;
            //printf("add->nex %d %d %d %d %d\npos %d\n", i, nex_station[i].Vj, nex_station[i].Vk, nex_station[i].Qj, nex_station[i].Qk, nex_station[i].pos);
            nex_station[i].busy = true;
            nex_station[i].ready = false;
            return true;
        }
    }
    return false;
}

void ReservationStation::Execute(Memory *memory, InstructionUnit *instructionUnit) {
    int minn = 2e9;
    int mini = -1;
    for (int i = 0; i < StationSize; i++) {
//        if (instructionUnit->Stall) {
//            printf("i %d %d %d %d\n",i,station[i].busy,station[i].ready,station[i].name);
//            std::cout << station[i].Qj << ' ' << station[i].Qk << '\n';
//            printf("nexi %d %d\n",nex_station[i].busy, nex_station[i].ready);
//            usleep(5e4);
//        }
        if (!station[i].busy) continue;
        switch(station[i].name) {
            case InstructionName::LB:
            case InstructionName::LH:
            case InstructionName::LW:
            case InstructionName::SB:
            case InstructionName::SH:
            case InstructionName::SW:
            case InstructionName::LBU:
            case InstructionName::LHU: {
                if (station[i].clock < minn) {
                    minn = station[i].clock;
                    mini = i;
                }
                break;
            }
            default:
                break;
        }
        if (station[i].ready) continue;
        if (~station[i].Qj) continue;
        if (~station[i].Qk) continue;
//        printf("i bob %d\n",i);
        switch (station[i].name) {
            case InstructionName::ADD:
            case InstructionName::SUB:
            case InstructionName::ADDI: {
                for (int j = 0; j < ALUSize; j++) {
                    if ((!addALU[j].busy) && (!addALU[j].ready)) {
                        nex_station[i].ready = true;
                        addALU[j].Execute(station[i].name, i, station[i].Vj, station[i].Vk, 1);
                        break;
                    }
                }
                break;
            }
            case InstructionName::SLL:
            case InstructionName::SRL:
            case InstructionName::SRA:
            case InstructionName::SLLI:
            case InstructionName::SRLI:
            case InstructionName::SRAI: {
                for (int j = 0; j < ALUSize; j++) {
                    if ((!shiftALU[j].busy) && (!shiftALU[j].ready)) {
                        nex_station[i].ready = true;
                        shiftALU[j].Execute(station[i].name, i, station[i].Vj, station[i].Vk, 1);
                        break;
                    }
                }
                break;
            }
            case InstructionName::OR:
            case InstructionName::XOR:
            case InstructionName::AND:
            case InstructionName::ORI:
            case InstructionName::XORI:
            case InstructionName::ANDI: {
                for (int j = 0; j < ALUSize; j++) {
                    if ((!bitALU[j].busy) && (!bitALU[j].ready)) {
                        nex_station[i].ready = true;
                        bitALU[j].Execute(station[i].name, i, station[i].Vj, station[i].Vk, 1);
                        break;
                    }
                }
                break;
            }
            case InstructionName::SLT:
            case InstructionName::BEQ:
            case InstructionName::BNE:
            case InstructionName::BLT:
            case InstructionName::BGE:
            case InstructionName::BLTU:
            case InstructionName::BGEU:
            case InstructionName::SLTI:
            case InstructionName::SLTU:
            case InstructionName::SLTIU: {
                for (int j = 0; j < ALUSize; j++) {
                    if ((!compALU[j].busy) && (!compALU[j].ready)) {
                        //printf("comp %d %s\n",j, getEnumName(station[i].name));
                        nex_station[i].ready = true;
                        compALU[j].Execute(station[i].name, i, station[i].Vj, station[i].Vk, 1);
                        break;
                    }
                }
                break;
            }
            case InstructionName::JALR: {// only 1 cycle
                //std::cout << "JALR EX!\n";
                instructionUnit->PC = station[i].Vj & (~1);
                instructionUnit->Stall = false;
                //std::cout << "nowPC " << std::dec << instructionUnit->PC << '\n';
                nex_station[i].busy = nex_station[i].ready = false;
                break;
            }
            case InstructionName::LB:
            case InstructionName::LH:
            case InstructionName::LW:
            case InstructionName::SB:
            case InstructionName::SH:
            case InstructionName::SW:
            case InstructionName::LBU:
            case InstructionName::LHU:
                break;
            default:
                throw Exception("Wrong Instruction in RS");
        }
    }
    if (~mini) {
        if (station[mini].ready) return ;
        if (~station[mini].Qj) return ;
        if (~station[mini].Qk) return ;
        if (!LSU.done) {
            nex_station[mini].ready = true;
            LSU.Execute(station[mini].name, mini, station[mini].Vj, station[mini].Vk, 3, memory);
        }
    }
}

void ReservationStation::Return(ReorderBuffer *reorderBuffer) {
    LSU.Return(reorderBuffer, this);
    for (int i = 0; i < ALUSize; i++) {
        addALU[i].Return(reorderBuffer, this);
        shiftALU[i].Return(reorderBuffer, this);
        bitALU[i].Return(reorderBuffer, this);
        compALU[i].Return(reorderBuffer, this);
    }
}

void ReservationStation::Update(const ReorderBuffer *reorderBuffer) {
    for (int i = 0; i < StationSize; i++) {
        if ((!station[i].busy) || station[i].ready) continue;
        if ((~station[i].Qj) && (*reorderBuffer)[station[i].Qj].ready) {
            nex_station[i].Vj = (*reorderBuffer)[station[i].Qj].val;
            nex_station[i].Qj = -1;
        }
        if ((~station[i].Qk) && (*reorderBuffer)[station[i].Qk].ready) {
            nex_station[i].Vk = static_cast<DataUnit>(static_cast<SignedDataUnit>(nex_station[i].Vk) +
                                                     (*reorderBuffer)[station[i].Qk].val);
            nex_station[i].Qk = -1;
        }
    }
}


#endif //RISC_V_SIMULATOR_RESERVATIONSTATION_CPP
