#include <vector>
#include <iostream>
#include <optional>

#include "vm_handlers.h"
#include "disassembler.h"
#include "vm_info.h"


Instruction::Instruction(OpCode op, uint8_t sz, uint32_t addr, std::vector<uint32_t> ops) : opcode(op), size(sz), address(addr), operands(std::move(ops))
{
}

Disassembler::Disassembler(const std::vector<uint8_t> vm_code_) : vm_code(std::move(vm_code_)), IP(0) 
{
}

uint8_t Disassembler::getRegUpper(uint8_t operand)
{
    return operand >> 4;
}

uint8_t Disassembler::getRegLower(uint8_t operand)
{
    return operand & 0xF;
}

std::optional<Instruction> Disassembler::getInstruction(const std::vector<uint8_t>& vm_code, uint32_t IP)
{
    uint8_t reg_upper, reg_lower, data;
    uint32_t jmp_target, value = 0;

    if (IP >= static_cast<uint32_t>(vm_code.size()))
    {
        return std::nullopt;
    }

    auto opcode = static_cast<OpCode>(vm_code[IP]); 
    switch (opcode)
    {
        case OpCode::RET:
            return Instruction(opcode, 1, IP, {});    

        case OpCode::SUB:
        case OpCode::ADD:    
        case OpCode::MUL:
        case OpCode::DIV:
        case OpCode::XOR:
        case OpCode::AND:
        case OpCode::MOV:
        case OpCode::CMP:
            data = vm_code[IP + 1];
            reg_upper = this->getRegUpper(data);
            reg_lower = this->getRegLower(data);
            return Instruction(opcode, 2, IP, {reg_upper, reg_lower});

        case OpCode::INC:
        case OpCode::DEC:
        case OpCode::PUSHREG:
        case OpCode::SETUSERDATA:
        case OpCode::GETUSERDATA:
        case OpCode::POP:
            data = vm_code[IP + 1];
            reg_upper = this->getRegUpper(data);
            return Instruction(opcode, 2, IP, {reg_upper});

        case OpCode::PUSHDATA:
            value = ((uint32_t)vm_code[IP + 1] << 24) | ((uint32_t)vm_code[IP + 2] << 16) | ((uint32_t)vm_code[IP + 3] << 8) | (uint32_t)vm_code[IP + 4];
            return Instruction(opcode, 5, IP, {value});
            
        case OpCode::LOOP:
            jmp_target = IP - vm_code[IP + 1]; 
            return Instruction(opcode, 2, IP, {jmp_target});

        case OpCode::JL:
        case OpCode::JG:
        case OpCode::JE:
            jmp_target = IP + vm_code[IP + 1] + 2; 
            return Instruction(opcode, 2, IP, {jmp_target});
            
        case OpCode::DECUSERDATA:
        case OpCode::INCUSERDATA:
            return Instruction(opcode, 1, IP, {});

        case OpCode::MODIFYNEXT16INSTRUCTIONS:
            return Instruction(opcode, 16, IP, {});
    }
    
    return std::nullopt;
}

std::optional<Instruction> Disassembler::getInstructionNext()
{
    auto insn = this->getInstruction(this->vm_code, this->IP);
    if (insn)
    {
        this->IP += insn->size;
    }

    return insn;
}

std::optional<Instruction> Disassembler::getInstructionAt(uint32_t IP)
{
    return this->getInstruction(this->vm_code, IP);
}

std::vector<Instruction> Disassembler::getAllInstructions()
{
    std::vector<Instruction> instructions;

    while (true)
    {
        auto insn = this->getInstructionNext();
        if (!insn)
            break;

        this->printInstruction(*insn);
        instructions.push_back(*insn);
    }

    return instructions;
}

bool Disassembler::isCondType(Instruction insn)
{
    return insn.opcode == OpCode::LOOP || insn.opcode == OpCode::JE || insn.opcode == OpCode::JG || insn.opcode == OpCode::JL;
}

const char* Disassembler::opcodeToString(OpCode opcode)
{
    switch (opcode)
    {
        case OpCode::RET: 
            return "RET";

        case OpCode::SUB: 
            return "SUB";
        
        case OpCode::ADD: 
            return "ADD";
        
        case OpCode::MUL: 
            return "MUL";
        case OpCode::DIV: 
            return "DIV";
        
        case OpCode::XOR: 
            return "XOR";
        
        case OpCode::AND: 
            return "AND";
        
        case OpCode::MOV: 
            return "MOV";
        
        case OpCode::CMP: 
            return "CMP";
        
        case OpCode::INC: 
            return "INC";
        
        case OpCode::DEC: 
            return "DEC";
        
        case OpCode::PUSHREG: 
            return "PUSHREG";
        
        case OpCode::PUSHDATA: 
            return "PUSHDATA";
        
        case OpCode::POP: 
            return "POP";
        
        case OpCode::SETUSERDATA: 
            return "SETUSERDATA";
        
        case OpCode::GETUSERDATA: 
            return "GETUSERDATA";
        
        case OpCode::LOOP: 
            return "LOOP";

        case OpCode::JL: 
            return "JL";
        
        case OpCode::JG: 
            return "JG";
        
        case OpCode::JE: 
            return "JE";
        
        case OpCode::DECUSERDATA: 
            return "DECUSERDATA";
        
        case OpCode::INCUSERDATA: 
            return "INCUSERDATA";
        
        case OpCode::MODIFYNEXT16INSTRUCTIONS: 
            return "MODNEXT16";
    }

    __assume(false);
}

void Disassembler::printInstruction(const Instruction& insn)
{
    std::printf("[%08X""] %-12s", insn.address, this->opcodeToString(insn.opcode));

    if (!insn.operands.empty())
    {
        for (size_t i = 0; i < insn.operands.size(); ++i)
        {
            std::printf(" 0x%X", insn.operands[i]);
        }
    }

    std::printf("\n");
}