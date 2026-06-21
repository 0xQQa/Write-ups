#include <iostream>
#include <optional>
#include <unordered_set>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/Utils/Mem2Reg.h>
#include <llvm/Transforms/Utils/LoopSimplify.h>
#include <llvm/Support/raw_ostream.h>

#include "vm_handlers.h"
#include "lifter.h"
#include "vm_info.h"
#include "disassembler.h"


Lifter::Lifter() : VmHandlers()
{
    llvm::Triple triple;
    triple.setArch(llvm::Triple::x86);
    triple.setVendor(llvm::Triple::PC);
    triple.setOS(llvm::Triple::Win32);
    triple.setEnvironment(llvm::Triple::MSVC);

    this->module->setTargetTriple(triple);
}

void Lifter::saveOutput(char *out_ll_filename)
{
    std::error_code      EC;
    llvm::raw_fd_ostream outputFile(out_ll_filename, EC);

    if (EC)
    {
        llvm::errs() << "[!] Error writing to file: " << EC.message() << "\n";
    }
    else
    {
        this->module->print(outputFile, nullptr);
    }
}

void Lifter::parseCFG(std::vector<Instruction> insns)
{
    std::unordered_set<uint64_t> all_addrs;

    for (const auto& insn : insns)
    {
        all_addrs.insert(insn.address);
    }
    
    for (const auto& insn : insns)
    {
        if (!Disassembler::isCondType(insn))
        {
            continue;
        }

        uint64_t target_addr = insn.operands[0];
        bool is_backedge = (target_addr <= insn.address) && (all_addrs.count(target_addr) > 0);

        if (this->addr_to_bb.find(target_addr) == this->addr_to_bb.end())
        {
            std::string name = is_backedge ? "loop_hdr_" + std::to_string(target_addr): "bb_" + std::to_string(target_addr);
            this->addr_to_bb[target_addr] = llvm::BasicBlock::Create(this->context, name, this->main_func);
        }

        if (&insn == &insns.back())
        {
            break;
        }

        uint64_t ft_addr = insn.address + insn.size;
        if (this->addr_to_bb.find(ft_addr) == this->addr_to_bb.end())
        {
            this->addr_to_bb[ft_addr] = llvm::BasicBlock::Create(this->context, "bb_" + std::to_string(ft_addr), this->main_func);
        }
    }
}

bool Lifter::isBlockTerminated(llvm::BasicBlock* bb)
{
    if (bb->empty()) 
    {
        return false;
    }
     
    return bb->back().isTerminator();
};

void Lifter::parseInstructions(std::vector<Instruction> insns)
{
    for (const auto& insn : insns)
    {
        auto it = this->addr_to_bb.find(insn.address);
        if (it != this->addr_to_bb.end())
        {
            if (!isBlockTerminated(this->builder.GetInsertBlock()))
            {
                this->builder.CreateBr(it->second);
            }
            
            this->builder.SetInsertPoint(it->second);
        }

        HandlerFn hndlr = this->getHandler(insn);
        (this->*hndlr)(insn);
    }
}

void Lifter::parseVmCode(std::vector<Instruction> insns)
{
    this->parseCFG(insns);
    this->parseInstructions(insns);
    llvm::verifyFunction(*this->main_func);
}