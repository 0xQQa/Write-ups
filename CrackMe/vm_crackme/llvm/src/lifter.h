#pragma once

#include <iostream>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "vm_handlers.h"


class Lifter : public VmHandlers
{    
    private:
        void parseCFG(std::vector<Instruction> insns);
        void parseInstructions(std::vector<Instruction> insns);
        bool isBlockTerminated(llvm::BasicBlock* bb);
        
    public:
        Lifter();
        void saveOutput(char *out_ll_filename);
        void parseVmCode(std::vector<Instruction> insns);
};