#include "merak16-as.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>

uint64_t inst_count = 0;

int main() {

    Assembler assembler;
    assembler.assembleFromFile("input.asm", "output.txt");

    std::cout << "Assembler successful." << std::endl;

    return 0;
}

void Assembler::assembleFromFile(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file '" << inputFileName << "'." << std::endl;
        return;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file '" << outputFileName << "'." << std::endl;
        inputFile.close();
        return;
    }

    // Read lines from input file, assemble instructions, and write machine code to output file

    std::string line;
    while (std::getline(inputFile, line)) {
        // Implement your assembler logic here
        // Process the 'line' and generate machine code
        // Write the machine code to the output file
        inst_count ++;

        size_t pos0 = line.find(" ");
        std::string opcode = line.substr(0, pos0);
        std::transform(opcode.begin(), opcode.end(), opcode.begin(), toupper);
        std::cout << "inst count: " << inst_count << ", opcode: " << opcode << std::endl;

        switch (OPCodeMapping[opcode])
        {
        case OPCode::AND:
        case OPCode::OR:
        case OPCode::XOR:
        case OPCode::ADD:
        case OPCode::SUB:
        case OPCode::SLL:
        case OPCode::SRA:
        case OPCode::SRL:
        {
            // Extract operands from the instruction
            // Assuming the format: and rd, rs1, rs2
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");
            size_t pos3 = line.find(", ", pos2 + 2);

            std::string rd = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs1 = line.substr(pos2 + 2, pos3 - pos2 - 2);
            std::string rs2 = line.substr(pos3 + 2);

            std::string machineCode = convertALUInstruction(OPCodeMapping[opcode], registerMapping[rs2], 
                registerMapping[rs1], registerMapping[rd], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::NOT:
        case OPCode::COM:
        {
            // Assuming the format: not rd, rs1
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");

            std::string rd = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs1 = line.substr(pos2 + 2);

            std::string machineCode = convertUNIInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], registerMapping[rd], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::MVHL:
        case OPCode::MVLH:
        case OPCode::MVH:
        {
            // Assuming the format: not rd, rs1
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");

            std::string rd = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs1 = line.substr(pos2 + 2);

            std::string machineCode = convertMVInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], registerMapping[rd], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::LH:
        {
            // Assuming the format: lh a5, 0x28(a4)
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");
            size_t pos3 = line.find("(", pos2 + 2);
            size_t pos4 = line.find(")", pos3 + 1);

            std::string rd = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs1 = line.substr(pos3 + 1, pos4 - pos3 - 1);
            std::string offset = line.substr(pos2 + 2, pos3 - pos2 - 1);

            std::string machineCode = convertLHInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], registerMapping[rd], std::strtol(offset.c_str(), nullptr, 16), line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::LI:
        {
            // Assuming the format: li a5, 0x28
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");

            std::string rd = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string offset = line.substr(pos2 + 2);

            std::string machineCode = convertLIInstruction(OPCodeMapping[opcode], 
                std::strtol(offset.c_str(), nullptr, 0), registerMapping[rd], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::SH:
        {
            // Assuming the format: sh a5, 0x28(a4)
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");
            size_t pos3 = line.find("(", pos2 + 2);
            size_t pos4 = line.find(")", pos3 + 1);

            std::string rs2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs1 = line.substr(pos3 + 1, pos4 - pos3 - 1);
            std::string offset = line.substr(pos2 + 2, pos3 - pos2 - 1);

            std::string machineCode = convertSHInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], registerMapping[rs2], std::strtol(offset.c_str(), nullptr, 16), line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::SLT:
        case OPCode::SOE:
        {
            // Assuming the format: slt a2, a3
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");

            std::string rs1 = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string rs2 = line.substr(pos2 + 2);

            std::string machineCode = convertSetCOMSInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], registerMapping[rs2], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::BOZ:
        case OPCode::BONZ:
        case OPCode::JAL:
        {
            // Assuming the format: boz 0x58
            size_t pos1 = line.find(" ");

            std::string offset = line.substr(pos1 + 1);

            std::string machineCode = convertBRInstruction(OPCodeMapping[opcode], 
                std::strtol(offset.c_str(), nullptr, 16), line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::JALR:
        {
            // Assuming the format: sh a5, 0x28
            size_t pos1 = line.find(" ");
            size_t pos2 = line.find(", ");

            std::string rs1 = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string offset = line.substr(pos2 + 2);

            std::string machineCode = convertJALRInstruction(OPCodeMapping[opcode], 
                registerMapping[rs1], std::strtol(offset.c_str(), nullptr, 16), line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        case OPCode::HALT:
        {
            // Assuming the format: halt

            std::string machineCode = convertHALTInstruction(OPCodeMapping[opcode], line);

            // Write the machine code to the output file
            outputFile << machineCode << std::endl;
            break;
        }
        default:
            std::cout << "ERROR: " << line << std::endl <<
                " invalid instruction" << std::endl;
            exit(EXIT_FAILURE);
            break;
        }

        std::string assembledMachineCode = line + "123";

        // outputFile << assembledMachineCode << std::endl;

        std::cout << line << std::endl;
    }

    // Close file streams
    inputFile.close();
    outputFile.close();
}

std::string Assembler::convertALUInstruction(OPCode opcode, 
    int16_t rs2, int16_t rs1, int16_t rd, const std::string& ori_asm) {
    // Combine the fields to generate the machine code
    if ((rd ^ rs1) >> 3 || (rs2 ^ rs1) >> 3 || (rd ^ rs2) >> 3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "rd, rs1 and rs2 are not in the same group" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string ALUFunctionStr;
    switch (opcode)
    {
        case OPCode::AND:
            ALUFunctionStr = "000";
            break;
        case OPCode::OR:
            ALUFunctionStr = "001";
            break;
        case OPCode::XOR:
            ALUFunctionStr = "010";
            break;
        case OPCode::ADD:
            ALUFunctionStr = "011";
            break;
        case OPCode::SUB:
            ALUFunctionStr = "100";
            break;
        case OPCode::SLL:
            ALUFunctionStr = "101";
            break;
        case OPCode::SRA:
            ALUFunctionStr = "110";
            break;
        case OPCode::SRL:
            ALUFunctionStr = "111";
            break;
        default:
            std::cout << "ERROR: unsupport ALU function code" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    std::string machineCode = std::string("000") + "_" + ALUFunctionStr + "_" + 
        std::to_string(rd >> 3) + "_" + 
        std::bitset<3>(rs2).to_string() + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(rd).to_string() + "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertUNIInstruction(OPCode opcode, 
    int16_t rs1, int16_t rd, const std::string& ori_asm) {

    // Combine the fields to generate the machine code
    if ((rd ^ rs1) >> 3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "rd and rs1 are not in the same group" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string UNIFunctionStr;
    switch (opcode)
    {
        case OPCode::NOT:
            UNIFunctionStr = "000";
            break;
        case OPCode::COM:
            UNIFunctionStr = "001";
            break;
        default:
            std::cout << "ERROR: unsupport UNI function code" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    std::string machineCode = std::string("001_000") + "_" + 
        std::to_string(rd >> 3) + "_" + 
        UNIFunctionStr + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(rd).to_string() + "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertMVInstruction(OPCode opcode, 
    int16_t rs1, int16_t rd, const std::string& ori_asm) {

    std::string MVFunctionStr;
    switch (opcode)
    {
        case OPCode::MVHL:
            if (!(rs1 >> 3) || (rd >> 3))
            {
                std::cout << "ERROR: " << ori_asm << std::endl <<
                    "rd or rs1 is not in the correct group" << std::endl;
                exit(EXIT_FAILURE);
            }
            MVFunctionStr = "0_010";
            break;
        case OPCode::MVLH:
            if ((rs1 >> 3) || !(rd >> 3))
            {
                std::cout << "ERROR: " << ori_asm << std::endl <<
                    "rd or rs1 is not in the correct group" << std::endl;
                exit(EXIT_FAILURE);
            }
            MVFunctionStr = "1_010";
            break;
        case OPCode::MVH:
            if (!(rs1 >> 3) || !(rd >> 3))
            {
                std::cout << "ERROR: " << ori_asm << std::endl <<
                    "rd or rs1 is not in the correct group" << std::endl;
                exit(EXIT_FAILURE);
            }
            MVFunctionStr = "1_011";
            break;
        default:
            std::cout << "ERROR: unsupport MV function code" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    std::string machineCode = std::string("001_000") + "_" + 
        MVFunctionStr + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(rd).to_string() + "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertLHInstruction(OPCode opcode, 
    int16_t rs1, int16_t rd, int16_t offset, const std::string& ori_asm) {
    if ((rd ^ rs1) >> 3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "rd and rs1 are not in the same group" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (offset > 4 || offset < -3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "offset out of the boundary" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string machineCode = std::string("001_001") + "_" + 
        std::to_string(rd >> 3) + "_" + 
        std::bitset<3>(offset).to_string() + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(rd).to_string() + "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertLIInstruction(OPCode opcode, int16_t imm, 
    int16_t rd, const std::string& ori_asm) {

    const int IMM_BIT = 9;
    if ((imm > (1 << (IMM_BIT - 1)) - 1) || (imm < -(1 << (IMM_BIT - 1))))
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "imm out of the boundary" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string machineCode = std::string("010") + "_" + 
        std::bitset<3>(imm >> 6).to_string() + "_" + 
        std::to_string(rd >> 3) + "_" + 
        std::bitset<3>(imm >> 3).to_string() + "_" + 
        std::bitset<3>(imm).to_string() + "_" + 
        std::bitset<3>(rd).to_string() + "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertSHInstruction(OPCode opcode, 
    int16_t rs1, int16_t rs2, int16_t offset, const std::string& ori_asm) {
    if ((rs2 ^ rs1) >> 3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "rd and rs1 are not in the same group" << std::endl;
        exit(EXIT_FAILURE);
    }

    const int OFFSET_BIT = 3;

    if ((offset > (1 << (OFFSET_BIT - 1)) - 1) || (offset < -(1 << (OFFSET_BIT - 1))))
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "offset out of the boundary" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string machineCode = std::string("001_010") + "_" + 
        std::to_string(rs2 >> 3) + "_" + 
        std::bitset<3>(rs2).to_string() + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(offset).to_string() + 
         "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertSetCOMSInstruction(OPCode opcode, 
    int16_t rs1, int16_t rs2, const std::string& ori_asm) {
    if ((rs2 ^ rs1) >> 3)
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "rs2 and rs1 are not in the same group" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string SetCOMSFunctionStr;
    switch (opcode)
    {
        case OPCode::SLT:
            SetCOMSFunctionStr = "000";
            break;
        case OPCode::SOE:
            SetCOMSFunctionStr = "001";
            break;
        default:
            std::cout << "ERROR: unsupport set COMS function code" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    std::string machineCode = std::string("100_000") + "_" + 
        std::to_string(rs2 >> 3) + "_" + 
        std::bitset<3>(rs2).to_string() + "_" +
        std::bitset<3>(rs1).to_string() + "_" + 
        SetCOMSFunctionStr + 
         "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertBRInstruction(OPCode opcode, int16_t offset, const std::string& ori_asm) {

    const int OFFSET_BIT = 10;

    if ((offset > (1 << (OFFSET_BIT - 1)) - 1) || (offset < -(1 << (OFFSET_BIT - 1))))
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "offset out of the boundary" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string SetBRFunctionStr;
    switch (opcode)
    {
        case OPCode::BOZ:
            SetBRFunctionStr = "100_001";
            break;
        case OPCode::BONZ:
            SetBRFunctionStr = "100_010";
            break;
        case OPCode::JAL:
            SetBRFunctionStr = "100_100";
            break;
        default:
            std::cout << "ERROR: unsupport branch function code" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    std::string machineCode = SetBRFunctionStr + "_" +
        std::bitset<1>(offset >> 9).to_string() + "_" +
        std::bitset<3>(offset >> 6).to_string() + "_" +
        std::bitset<3>(offset >> 3).to_string() + "_" +
        std::bitset<3>(offset).to_string() +
         "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertJALRInstruction(OPCode opcode, 
    int16_t rs1, int16_t offset, const std::string& ori_asm) {

    const int OFFSET_BIT = 6;

    if ((offset > (1 << (OFFSET_BIT - 1)) - 1) || (offset < -(1 << (OFFSET_BIT - 1))))
    {
        std::cout << "ERROR: " << ori_asm << std::endl <<
            "offset out of the boundary" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string machineCode = std::string("100_101") + "_" + 
        std::to_string(rs1 >> 3) + "_" + 
        std::bitset<3>(offset >> 3).to_string() + "_" + 
        std::bitset<3>(rs1).to_string() + "_" + 
        std::bitset<3>(offset).to_string() + 
         "  // " + ori_asm;

    return machineCode;
}

std::string Assembler::convertHALTInstruction(OPCode opcode, const std::string& ori_asm) {

    std::string machineCode = std::string("111_111_1_111_111_111") +
         "  // " + ori_asm;

    return machineCode;
}