//
// Created by 屋顶上的小丑 on 2023/6/21.
//

#ifndef RISC_V_SIMULATOR_INSTRUCTIONS_CPP
#define RISC_V_SIMULATOR_INSTRUCTIONS_CPP

#include "Instructions.h"
#include "ReorderBuffer.h"
#include "ReservationStation.h"

void InstructionUnit::StationInit(const InstructionName &name) {
    RSData.name = name;
    RSData.Qj = RSData.Qk = -1;
    RSData.Vj = RSData.Vk = 0;
    RSData.ready = false;
    RSData.busy = true;
}

void InstructionUnit::StationInitRegister(ReorderBuffer *reorderBuffer,
                                          RegisterFile *registerFile,
                                          const DataUnit &rs, bool type) {
    const SignedDataUnit &Dependency = (*registerFile)[rs].dependency;
    if (!type) {
        if (rs && (~Dependency)) {
            if ((*reorderBuffer)[Dependency].ready) {
                RSData.Vj = (*reorderBuffer)[Dependency].val;
            } else RSData.Qj = Dependency;
        } else RSData.Vj = (*registerFile)[rs].data;
    } else {
        if (rs && (~Dependency)) {
            if ((*reorderBuffer)[Dependency].ready) {
                RSData.Vk = (*reorderBuffer)[Dependency].val;
            } else RSData.Qk = Dependency;
        } else RSData.Vk = (*registerFile)[rs].data;
    }
}

void InstructionUnit::Issue(ReorderBuffer *reorderBuffer, ReservationStation *reservationStation,
                            RegisterFile *registerFile, Memory *memory, const DataUnit &clock) {
    if (Stall) return;
    if (reorderBuffer->full()) return;
    DataUnit code = memory->ReadDataUnit(PC);
    Instruction instruction = FetchInstruction(code);
    RobData.name = instruction.name;
    switch (instruction.name) {
        case InstructionName::ADD:
        case InstructionName::SUB:
        case InstructionName::SLL:
        case InstructionName::SLT:
        case InstructionName::SLTU:
        case InstructionName::XOR:
        case InstructionName::OR:
        case InstructionName::AND:
        case InstructionName::SRL:
        case InstructionName::SRA: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = false;
            RobData.pos = instruction.rd;
            RobData.val = 0;
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, false);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs2, true);
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            if (reservationStation->Add(RSData)) PC += 4;
            else reorderBuffer->pop_back();
            break;
        }
        case InstructionName::ADDI:
        case InstructionName::SLTI:
        case InstructionName::SLTIU:
        case InstructionName::XORI:
        case InstructionName::ORI:
        case InstructionName::ANDI:
        case InstructionName::SLLI:
        case InstructionName::SRLI:
        case InstructionName::SRAI: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = false;
            RobData.pos = instruction.rd;
            RobData.val = 0;
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, false);
            RSData.Vk = instruction.imm;
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            if (reservationStation->Add(RSData)) PC += 4;
            else reorderBuffer->pop_back();
            break;
        }
        case InstructionName::BEQ:
        case InstructionName::BNE:
        case InstructionName::BLT:
        case InstructionName::BGE:
        case InstructionName::BLTU:
        case InstructionName::BGEU: {
            RobData.type = CommitType::Branch;
            RobData.ready = false;
            RobData.PredictedAns = predictor.predict(PC);
            RobData.add = PC;
            RobData.val = 0;
            if (RobData.PredictedAns) {
                RobData.pos = PC + 4;
                PC += static_cast<SignedDataUnit>(instruction.imm);
            } else {
                RobData.pos = PC + static_cast<SignedDataUnit>(instruction.imm);
                PC += 4;
            }
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, false);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs2, true);
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            if (!reservationStation->Add(RSData)) {
                reorderBuffer->pop_back();
                PC = RobData.add;
            }
            break;
        }
        case InstructionName::LB:
        case InstructionName::LH:
        case InstructionName::LW:
        case InstructionName::LBU:
        case InstructionName::LHU: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = false;
            RobData.val = 0;
            RobData.pos = instruction.rd;
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, true);
            RSData.Vk = static_cast<DataUnit>(RSData.Vk +
                        static_cast<SignedDataUnit>(instruction.imm));
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            RSData.clock = clock;
            if (reservationStation->Add(RSData)) PC += 4;
            else reorderBuffer->pop_back();
            break;
        }
        case InstructionName::SB:
        case InstructionName::SH:
        case InstructionName::SW: {
            RobData.type = CommitType::MemoryWrite;
            RobData.ready = false;
            RobData.val = 0;
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs2, false);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, true);
            RSData.Vk = static_cast<DataUnit>(RSData.Vk +
                        static_cast<SignedDataUnit>(instruction.imm));
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            RSData.clock = clock;
            if (reservationStation->Add(RSData)) PC += 4;
            else reorderBuffer->pop_back();
            break;
        }
        case InstructionName::LUI: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = true;
            RobData.val = instruction.imm;
            RobData.pos = instruction.rd;
            reorderBuffer->Add(RobData, registerFile);
            PC += 4;
            break;
        }
        case InstructionName::AUIPC: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = true;
            RobData.val = PC + static_cast<SignedDataUnit>(instruction.imm);
            RobData.pos = instruction.rd;
            reorderBuffer->Add(RobData, registerFile);
            PC += 4;
            break;
        }
        case InstructionName::JAL: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = true;
            RobData.val = PC + 4;
            RobData.pos = instruction.rd;
            reorderBuffer->Add(RobData, registerFile);
            PC = PC + static_cast<SignedDataUnit>(instruction.imm);
            break;
        }
        case InstructionName::JALR: {
            RobData.type = CommitType::RegisterWrite;
            RobData.ready = true;
            RobData.val = PC + 4;
            RobData.pos = instruction.rd;
            StationInit(instruction.name);
            StationInitRegister(reorderBuffer, registerFile, instruction.rs1, false);
            RSData.Vj = static_cast<DataUnit>(RSData.Vj +
                        static_cast<SignedDataUnit>(instruction.imm));
            RSData.pos = reorderBuffer->Add(RobData, registerFile);
            if (reservationStation->Add(RSData)) Stall = true;
            else reorderBuffer->pop_back();
            break;
        }
        case InstructionName::END: {
            RobData.ready = true;
            RobData.type = CommitType::Done;
            reorderBuffer->Add(RobData, registerFile);
            Stall = true;
            break;
        }
    }
}

#endif //RISC_V_SIMULATOR_INSTRUCTIONS_CPP