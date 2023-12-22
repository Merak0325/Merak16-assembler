#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class Assembler {
public:
    Assembler(){};  // Constructor
    ~Assembler(){}; // Destructor

    // Add other public methods for your assembler functionalities
    void assembleFromFile(const std::string& inputFileName, const std::string& outputFileName);

    enum class OPCode{
        AND,
        OR,
        XOR,
        ADD,
        SUB,
        SLL,
        SRA,
        SRL,
        NOT,
        COM,
        MVHL,
        MVLH,
        MVH,
        LH,
        LI,
        SH,
        SLT,
        SOE,
        BOZ,
        BONZ,
        JAL,
        JALR,
        HALT
    };
    /*
    enum class OPCode{
        BINARY_OP,
        UNI_AND_LS_OP,
        LI_OP,
        CTRL_FLOW_OP = 4,
        HALT_OP = 7
    };*/

    std::unordered_map<std::string, int16_t> registerMapping = {
        {"a0", 0},
        {"a1", 1},
        {"a2", 2},
        {"a3", 3},
        {"a4", 4},
        {"a5", 5},
        {"a6", 6},
        {"a7", 7},
        {"a8", 8},
        {"a9", 9},
        {"a10", 10},
        {"a11", 11},
        {"a12", 12},
        {"a13", 13},
        {"a14", 14},
        {"a15", 15}
    };

    std::unordered_map<std::string, OPCode> OPCodeMapping = {
        {"AND",  OPCode::AND},
        {"OR",   OPCode::OR},
        {"XOR",  OPCode::XOR},
        {"ADD",  OPCode::ADD},
        {"SUB",  OPCode::SUB},
        {"SLL",  OPCode::SLL},
        {"SRA",  OPCode::SRA},
        {"SRL",  OPCode::SRL},
        {"NOT",  OPCode::NOT},
        {"COM",  OPCode::COM},
        {"MVHL", OPCode::MVHL},
        {"MVLH", OPCode::MVLH},
        {"MVH",  OPCode::MVH},
        {"LH",   OPCode::LH},
        {"LI",   OPCode::LI},
        {"SH",   OPCode::SH},
        {"SLT",  OPCode::SLT},
        {"SOE",  OPCode::SOE},
        {"BOZ",  OPCode::BOZ},
        {"JAL",  OPCode::JAL},
        {"JALR", OPCode::JALR},
        {"HALT", OPCode::HALT}
    };

private:
    // Add private members for your assembler, such as registers, memory, etc.
    std::string convertALUInstruction(OPCode opcode, int16_t rs2, int16_t rs1, int16_t rd, const std::string& ori_asm);
    std::string convertUNIInstruction(OPCode opcode, int16_t rs1, int16_t rd, const std::string& ori_asm);
    std::string convertMVInstruction(OPCode opcode, int16_t rs1, int16_t rd, const std::string& ori_asm);
    std::string convertLHInstruction(OPCode opcode, int16_t rs1, int16_t rd, int16_t offset, const std::string& ori_asm);
    std::string convertLIInstruction(OPCode opcode, int16_t imm, int16_t rd, const std::string& ori_asm);
    std::string convertSHInstruction(OPCode opcode, int16_t rs1, int16_t rs2, int16_t offset, const std::string& ori_asm);
    std::string convertSetCOMSInstruction(OPCode opcode, int16_t rs1, int16_t rs2, const std::string& ori_asm);
    std::string convertBRInstruction(OPCode opcode, int16_t offset, const std::string& ori_asm);
    std::string convertJALRInstruction(OPCode opcode, int16_t rs1, int16_t offset, const std::string& ori_asm);
    std::string convertHALTInstruction(OPCode opcode, const std::string& ori_asm);
};

#endif // ASSEMBLER_H
