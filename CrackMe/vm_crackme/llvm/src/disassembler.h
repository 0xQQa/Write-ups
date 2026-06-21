#pragma once

#include <vector>
#include <iostream>
#include <optional>


enum class OpCode : uint8_t 
{
    RET                      = 0x66,
    ADD                      = 0x67,
    SUB                      = 0x68,
    MUL                      = 0x69,
    DIV                      = 0x6A,
    INC                      = 0x6B,
    DEC                      = 0x6C,
    XOR                      = 0x6D,
    AND                      = 0x6E,
    PUSHREG                  = 0x6F,
    PUSHDATA                 = 0x70,
    POP                      = 0x71,
    MOV                      = 0x72,
    GETUSERDATA              = 0x73,
    SETUSERDATA              = 0x74,
    LOOP                     = 0x75,
    CMP                      = 0x76,
    JL                       = 0x77,
    JG                       = 0x78,
    JE                       = 0x79,
    INCUSERDATA              = 0x7A,
    DECUSERDATA              = 0x7B,
    MODIFYNEXT16INSTRUCTIONS = 0x7C,
};

struct Instruction
{
    OpCode                  opcode;
    uint8_t                 size;
    uint32_t                address;
    std::vector<uint32_t>   operands;
    
    Instruction(OpCode op, uint8_t sz, uint32_t addr, std::vector<uint32_t> ops);
};

class Disassembler
{
    private:
        uint32_t IP;
        const std::vector<uint8_t> vm_code;
        std::optional<Instruction> getInstruction(const std::vector<uint8_t>& vm_code, uint32_t IP);
        uint8_t getRegUpper(uint8_t operand);
        uint8_t getRegLower(uint8_t operand);
        const char* opcodeToString(OpCode opcode);

    public:
        Disassembler(const std::vector<uint8_t> vm_code);
        std::optional<Instruction> getInstructionNext();
        std::optional<Instruction> getInstructionAt(uint32_t IP);
        std::vector<Instruction> getAllInstructions();
        static bool isCondType(Instruction insn);
        void printInstruction(const Instruction& insn);
};


