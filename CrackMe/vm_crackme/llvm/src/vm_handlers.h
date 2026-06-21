#pragma once

#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "disassembler.h"
#include "vm_info.h"


#define _LLVM_ALLIG_MEM_PAD_SIZE(X)     X->setAlignment(llvm::Align(MEM_PAD_SIZE))
#define REG_UPPER                       0
#define REG_LOWER                       1 

class VmHandlers
{
    private:
        using Common2RegOps = std::function<llvm::Value*(llvm::Value*, llvm::Value*)>;

        llvm::Value* getReg(uint8_t reg_idx, bool use_ptr);    
        llvm::Value* getInsReg(Instruction insn, uint8_t operand_idx);
        llvm::Value* getInsRegPtr(Instruction insn, uint8_t operand_idx);
        
        llvm::Value* loadVspOffset();
        void storeVspOffset(llvm::Value* offset);

        llvm::Value* loadBuffpOffset();
        void storeBuffpOffset(llvm::Value* offset);

        llvm::Value* offsetToPtr(llvm::Value* offset, llvm::AllocaInst* memory);
        void VmHandlers::opJx(Instruction insn, int32_t cmp_val);
        void opRegUpRegLowToRegUp(Instruction insn, Common2RegOps target_func);
        void IncDec(Instruction insn, int8_t value);

        void opAdd(Instruction insn);
        void opSub(Instruction insn);
        void opMul(Instruction insn);
        void opDiv(Instruction insn);
        void opInc(Instruction insn);
        void opDec(Instruction insn);
        void opXor(Instruction insn);
        void opAnd(Instruction insn);        
        void opPushReg(Instruction insn);
        void opPushData(Instruction insn);
        void opPop(Instruction insn);
        void opMov(Instruction insn);
        void opGetUserData(Instruction insn);
        void opSetUserData(Instruction insn);
        void opLoop(Instruction insn);
        void opCmp(Instruction insn);
        void opJl(Instruction insn);
        void opJg(Instruction insn);
        void opJe(Instruction insn);
        void opIncUserData(Instruction insn);
        void opDecUserData(Instruction insn);
        void opModifyNext16Instructions(Instruction insn);
        void opRet(Instruction insn);

    protected:
        using HandlerFn = void (VmHandlers::*)(Instruction);
        
        llvm::Function*                                     main_func;
        llvm::LLVMContext                                   context;
        std::unique_ptr<llvm::Module>                       module;
        llvm::IRBuilder<>                                   builder;
        llvm::Value*                                        vsp;
        llvm::AllocaInst*                                   stack;
        llvm::Value*                                        buffp;
        llvm::AllocaInst*                                   buff;
        std::vector<llvm::Value*>                           regs;
        std::unordered_map<uint64_t, llvm::BasicBlock*>     addr_to_bb;

        HandlerFn getHandler(Instruction insn);
        VmHandlers();            
};