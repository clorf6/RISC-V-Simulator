//
// Created by 屋顶上的小丑 on 2023/6/26.
//

#ifndef RISC_V_SIMULATOR_UTILS_CPP
#define RISC_V_SIMULATOR_UTILS_CPP

#include "Utils.h"

Byte FetchOpcode(const DataUnit &code) {
    return code & (0x7F);
}

DataUnit FetchRd(const DataUnit &code) {
    return (code >> 7) & (0x1F);
}

Byte FetchFunct3(const DataUnit &code) {
    return (code >> 12) & (0x07);
}

DataUnit FetchRs1(const DataUnit &code) {
    return (code >> 15) & (0x1F);
}

DataUnit FetchRs2(const DataUnit &code) {
    return (code >> 20) & (0x1F);
}

DataUnit FetchFunct7(const DataUnit &code) {
    return (code >> 25) & (0x7F);
}

DataUnit FetchIimm(const DataUnit &code) {
    return (code >> 20) & (0xFFF);
}

DataUnit FetchSimm(const DataUnit &code) {
    DataUnit imm = (code >> 7) & (0x1F);
    imm |= (code >> 20) & (0xFE0);
    return imm;
}

DataUnit FetchBimm(const DataUnit &code) {
    DataUnit imm = (code >> 7) & (0x1E);
    imm |= (code >> 20) & (0x7E0);
    imm |= (code << 4) & (0x800);
    imm |= (code >> 19) & (0x1000);
    return imm;
}

DataUnit FetchUimm(const DataUnit &code) {
    return code & (0xFFFFF000);
}

DataUnit FetchJimm(const DataUnit &code) {
    DataUnit imm = (code >> 20) & (0x7FE);
    imm |= (code >> 9) & (0x800);
    imm |= code & (0xFF000);
    imm |= (code >> 11) & (0x100000);
    return imm;
}

DataUnit FetchShamt(const DataUnit &code) {
    return (code >> 20) & (0x1F);
}

DataUnit ISSignedExtend(const DataUnit &imm) { // imm[11:0]
    return (imm & 0x0800) ? (imm | 0xFFFFF000) : imm;
}

DataUnit BSignedExtend(const DataUnit &imm) { // imm[12:0]
    return (imm & 0x1000) ? (imm | 0xFFFFE000) : imm;
}

DataUnit JSignedExtend(const DataUnit &imm) { // imm[20:0]
    return (imm & 0x100000) ? (imm | 0xFFE00000) : imm;
}

InstructionName FetchRInstruction(const DataUnit &code) {
    Byte Funct3 = FetchFunct3(code);
    DataUnit Funct7 = FetchFunct7(code);
    switch (Funct3) {
        case 0x0: {
            switch (Funct7) {
                case 0x00:
                    return InstructionName::ADD;
                case 0x20:
                    return InstructionName::SUB;
                default:
                    throw Exception("Wrong Instruction in Fetch");
            }
        }
        case 0x1:
            return InstructionName::SLL;
        case 0x2:
            return InstructionName::SLT;
        case 0x3:
            return InstructionName::SLTU;
        case 0x4:
            return InstructionName::XOR;
        case 0x5: {
            switch (Funct7) {
                case 0x00:
                    return InstructionName::SRL;
                case 0x20:
                    return InstructionName::SRA;
                default:
                    throw Exception("Wrong Instruction in Fetch");
            }
        }
        case 0x6:
            return InstructionName::OR;
        case 0x7:
            return InstructionName::AND;
        default:
            throw Exception("Wrong Instruction in Fetch");
    }
}

InstructionName FetchIInstruction(const DataUnit &code) {
    DataUnit Opcode = FetchOpcode(code);
    Byte Funct3 = FetchFunct3(code);
    DataUnit Funct7 = FetchFunct7(code);
    if (Opcode == 0x13) {
        switch (Funct3) {
            case 0x0:
                return InstructionName::ADDI;
            case 0x1:
                return InstructionName::SLLI;
            case 0x2:
                return InstructionName::SLTI;
            case 0x3:
                return InstructionName::SLTIU;
            case 0x4:
                return InstructionName::XORI;
            case 0x5: {
                switch (Funct7) {
                    case 0x00:
                        return InstructionName::SRLI;
                    case 0x20:
                        return InstructionName::SRAI;
                    default:
                        throw Exception("Wrong Instruction in Fetch");
                }
            }
            case 0x6:
                return InstructionName::ORI;
            case 0x7:
                return InstructionName::ANDI;
            default:
                throw Exception("Wrong Instruction in Fetch");
        }
    } else {
        switch (Funct3) {
            case 0x0:
                return InstructionName::LB;
            case 0x1:
                return InstructionName::LH;
            case 0x2:
                return InstructionName::LW;
            case 0x4:
                return InstructionName::LBU;
            case 0x5:
                return InstructionName::LHU;
            default:
                throw Exception("Wrong Instruction in Fetch");
        }
    }
}

InstructionName FetchSInstruction(const DataUnit &code) {
    Byte Funct3 = FetchFunct3(code);
    switch (Funct3) {
        case 0x0:
            return InstructionName::SB;
        case 0x1:
            return InstructionName::SH;
        case 0x2:
            return InstructionName::SW;
        default:
            throw Exception("Wrong Instruction in Fetch");
    }
}

InstructionName FetchBInstruction(const DataUnit &code) {
    Byte Funct3 = FetchFunct3(code);
    switch (Funct3) {
        case 0x0:
            return InstructionName::BEQ;
        case 0x1:
            return InstructionName::BNE;
        case 0x4:
            return InstructionName::BLT;
        case 0x5:
            return InstructionName::BGE;
        case 0x6:
            return InstructionName::BLTU;
        case 0x7:
            return InstructionName::BGEU;
        default:
            throw Exception("Wrong Instruction in Fetch");
    }
}

Instruction FetchInstruction(const DataUnit &code) {
    Instruction ret;
    if (code == 0x0FF00513) {
        ret.name = InstructionName::END;
        return ret;
    }
    Byte Opcode = FetchOpcode(code);
    switch (Opcode) {
        case 0x33: {
            ret.name = FetchRInstruction(code);
            ret.rd = FetchRd(code);
            ret.rs1 = FetchRs1(code);
            ret.rs2 = FetchRs2(code);
            break;
        }
        case 0x13: {
            ret.name = FetchIInstruction(code);
            ret.rd = FetchRd(code);
            ret.rs1 = FetchRs1(code);
            if (ret.name == InstructionName::SLLI ||
                ret.name == InstructionName::SRLI ||
                ret.name == InstructionName::SRAI) {
                ret.imm = FetchShamt(code);
            } else if (ret.name == InstructionName::SLTIU) {
                ret.imm = FetchIimm(code);
            } else ret.imm = ISSignedExtend(FetchIimm(code));
            break;
        }
        case 0x03: {
            ret.name = FetchIInstruction(code);
            ret.rd = FetchRd(code);
            ret.rs1 = FetchRs1(code);
            ret.imm = ISSignedExtend(FetchIimm(code));
            break;
        }
        case 0x23: {
            ret.name = FetchSInstruction(code);
            ret.rs1 = FetchRs1(code);
            ret.rs2 = FetchRs2(code);
            ret.imm = ISSignedExtend(FetchSimm(code));
            break;
        }
        case 0x63: {
            ret.name = FetchBInstruction(code);
            ret.rs1 = FetchRs1(code);
            ret.rs2 = FetchRs2(code);
            ret.imm = BSignedExtend(FetchBimm(code));
            break;
        }
        case 0x37: {
            ret.name = InstructionName::LUI;
            ret.rd = FetchRd(code);
            ret.imm = FetchUimm(code);
            break;
        }
        case 0x17: {
            ret.name = InstructionName::AUIPC;
            ret.rd = FetchRd(code);
            ret.imm = FetchUimm(code);
            break;
        }
        case 0x6F: {
            ret.name = InstructionName::JAL;
            ret.rd = FetchRd(code);
            ret.imm = JSignedExtend(FetchJimm(code));
            break;
        }
        case 0x67: {
            ret.name = InstructionName::JALR;
            ret.rd = FetchRd(code);
            ret.rs1 = FetchRs1(code);
            ret.imm = ISSignedExtend(FetchIimm(code));
            break;
        }
        default:
            throw Exception("Wrong Instruction in Fetch");
    }
    return ret;
}

const char* getEnumName(const InstructionName& value) {
    switch (value) {
        // R types
        case ADD: return "ADD";
        case SUB: return "SUB";
        case SLL: return "SLL";
        case SLT: return "SLT";
        case SLTU: return "SLTU";
        case XOR: return "XOR";
        case SRL: return "SRL";
        case SRA: return "SRA";
        case OR: return "OR";
        case AND: return "AND";
            // I types
        case ADDI: return "ADDI";
        case SLTI: return "SLTI";
        case SLTIU: return "SLTIU";
        case XORI: return "XORI";
        case ORI: return "ORI";
        case ANDI: return "ANDI";
        case SLLI: return "SLLI";
        case SRLI: return "SRLI";
        case SRAI: return "SRAI";
        case LB: return "LB";
        case LH: return "LH";
        case LW: return "LW";
        case LBU: return "LBU";
        case LHU: return "LHU";
            // S types
        case SB: return "SB";
        case SH: return "SH";
        case SW: return "SW";
            // B types
        case BEQ: return "BEQ";
        case BNE: return "BNE";
        case BLT: return "BLT";
        case BGE: return "BGE";
        case BLTU: return "BLTU";
        case BGEU: return "BGEU";
            // U types
        case LUI: return "LUI";
        case AUIPC: return "AUIPC";
            // J types
        case JAL: return "JAL";
        case JALR: return "JALR";
            // Others
        case END: return "END";
        default: return "Unknown";
    }
}

#endif //RISC_V_SIMULATOR_UTILS_CPP