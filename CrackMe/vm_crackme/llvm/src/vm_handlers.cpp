#include <iostream>
#include <string>

#include "disassembler.h"
#include "vm_handlers.h"
#include "vm_info.h"


VmHandlers::VmHandlers() : module(std::make_unique<llvm::Module>(VM_MODULE_NAME, context)), builder(context)
{
    llvm::FunctionType* funcType = llvm::FunctionType::get(this->builder.getInt32Ty(), {this->builder.getInt8Ty()->getPointerTo()}, false);
    this->main_func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "dispatcher", module.get());

    llvm::BasicBlock* entryBB = llvm::BasicBlock::Create(context, "entry_bb", this->main_func);
    this->builder.SetInsertPoint(entryBB);
    
    llvm::ArrayType* stackArrayType = llvm::ArrayType::get(this->builder.getInt8Ty(), STACK_SIZE * MEM_PAD_SIZE + STACK_HEADROOM);
    
    this->stack = this->builder.CreateAlloca(stackArrayType, nullptr);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->stack);
    
    this->vsp = this->builder.CreateAlloca(this->builder.getInt32Ty(), nullptr);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(this->builder.getInt32(STACK_SIZE * MEM_PAD_SIZE), this->vsp));

    this->regs = std::vector<llvm::Value*>(REGS_COUNT, nullptr);
    for (int reg_idx = 0; reg_idx < this->regs.size(); ++reg_idx)
    {
        this->regs[reg_idx] = this->builder.CreateAlloca(builder.getInt32Ty(), nullptr);
        _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(llvm::ConstantInt::get(this->builder.getInt32Ty(), 0), regs[reg_idx]));
    }

    llvm::ArrayType* buffArrayType = llvm::ArrayType::get(this->builder.getInt8Ty(), USER_BUFF_SIZE);
    
    this->buff = this->builder.CreateAlloca(buffArrayType, nullptr);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->buff);
    
    this->buffp = this->builder.CreateAlloca(this->builder.getInt32Ty(), nullptr);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(this->builder.getInt32(0), this->buffp));

    llvm::Value* input = this->main_func->getArg(0);
    llvm::Value* buff0 = this->builder.CreateGEP(buffArrayType, this->buff, {this->builder.getInt32(0), this->builder.getInt32(0)});    

    this->builder.CreateMemCpy(buff0, llvm::MaybeAlign(4), input, llvm::MaybeAlign(1), this->builder.getInt32(USER_BUFF_SIZE));
}

llvm::Value* VmHandlers::getReg(uint8_t reg_idx, bool use_ptr)
{
    if (reg_idx > REGS_COUNT - 1)
    {
        __assume(false);
    }

    if (use_ptr)
    {
        return this->regs[reg_idx];
    }

    return this->builder.CreateLoad(this->builder.getInt32Ty(), this->regs[reg_idx], "reg_" + std::to_string(reg_idx) + "_");
}

llvm::Value* VmHandlers::getInsReg(Instruction insn, uint8_t operand_idx)
{
    uint8_t reg_idx = insn.operands[operand_idx];
    return this->getReg(reg_idx, false);
}

llvm::Value* VmHandlers::getInsRegPtr(Instruction insn, uint8_t operand_idx)
{
    uint8_t reg_idx = insn.operands[operand_idx];
    return this->getReg(reg_idx, true);
}

llvm::Value* VmHandlers::loadVspOffset()
{
    auto val = this->builder.CreateLoad(this->builder.getInt32Ty(), this->vsp, "vsp_off");
    _LLVM_ALLIG_MEM_PAD_SIZE(val);
    return val;
}

void VmHandlers::storeVspOffset(llvm::Value* offset)
{
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(offset, this->vsp));
}

llvm::Value* VmHandlers::loadBuffpOffset()
{
    auto val = this->builder.CreateLoad(this->builder.getInt32Ty(), this->buffp, "buff_off");
    _LLVM_ALLIG_MEM_PAD_SIZE(val);
    return val;
}

void VmHandlers::storeBuffpOffset(llvm::Value* offset)
{
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(offset, this->buffp));
}

llvm::Value* VmHandlers::offsetToPtr(llvm::Value* offset, llvm::AllocaInst* memory)
{
    return this->builder.CreateGEP(this->builder.getInt8Ty(), memory, offset);
}

void VmHandlers::opPushReg(Instruction insn)
{
    auto reg = this->getInsReg(insn, REG_UPPER);
    llvm::Value* off = this->loadVspOffset();
    llvm::Value* new_off = this->builder.CreateSub(off, this->builder.getInt32(MEM_PAD_SIZE));
    storeVspOffset(new_off);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(reg, offsetToPtr(new_off, this->stack)));
}

void VmHandlers::opPushData(Instruction insn)
{
    auto imm_value = this->builder.getInt32(insn.operands[0]);
    llvm::Value* off = this->loadVspOffset();
    llvm::Value* new_off = this->builder.CreateSub(off, this->builder.getInt32(MEM_PAD_SIZE));
    storeVspOffset(new_off);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(imm_value, offsetToPtr(new_off, this->stack)));    
}

void VmHandlers::opPop(Instruction insn)
{
    auto reg_ptr = this->getInsRegPtr(insn, REG_UPPER);
    llvm::Value* off = this->loadVspOffset();
    llvm::Value* new_off = this->builder.CreateAdd(off, this->builder.getInt32(MEM_PAD_SIZE));
    llvm::LoadInst* val = this->builder.CreateLoad(this->builder.getInt32Ty(), offsetToPtr(off, this->stack));
    _LLVM_ALLIG_MEM_PAD_SIZE(val);
    storeVspOffset(new_off);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(val, reg_ptr));
}

void VmHandlers::opRegUpRegLowToRegUp(Instruction insn, Common2RegOps target_func)
{
    auto reg_upper = this->getInsReg(insn, REG_UPPER);
    auto reg_lower = this->getInsReg(insn, REG_LOWER);
    auto reg_upper_ptr = this->getInsRegPtr(insn, REG_UPPER);

    auto result = target_func(reg_upper, reg_lower);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(result, reg_upper_ptr));
}

void VmHandlers::opAdd(Instruction insn)
{
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateAdd(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);
}

void VmHandlers::opSub(Instruction insn)
{   
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateSub(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);
}

void VmHandlers::opMul(Instruction insn)
{    
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateMul(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);
}

void VmHandlers::opDiv(Instruction insn)
{    
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateUDiv(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);    
}

void VmHandlers::opXor(Instruction insn)
{    
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateXor(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);
}

void VmHandlers::opAnd(Instruction insn)
{    
    auto target_func = [this](llvm::Value* r_u, llvm::Value* r_l) { return this->builder.CreateAnd(r_u, r_l); };
    this->opRegUpRegLowToRegUp(insn, target_func);
}

void VmHandlers::IncDec(Instruction insn, int8_t value)
{
    auto reg_upper = this->getInsReg(insn, REG_UPPER);
    auto reg_upper_ptr = this->getInsRegPtr(insn, REG_UPPER);
 
    auto result = this->builder.CreateAdd(reg_upper, this->builder.getInt32(value));
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(result, reg_upper_ptr));
}

void VmHandlers::opInc(Instruction insn)
{    
    this->IncDec(insn, 1);
}

void VmHandlers::opDec(Instruction insn)
{    
    this->IncDec(insn, -1);
}

void VmHandlers::opMov(Instruction insn)
{    
    auto reg_upper_ptr = this->getInsRegPtr(insn, REG_UPPER);
    auto reg_lower_ptr = this->getInsRegPtr(insn, REG_LOWER);
    
    auto result = this->builder.CreateLoad(this->builder.getInt32Ty(), reg_lower_ptr);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(result, reg_upper_ptr));
}

void VmHandlers::opCmp(Instruction insn)
{
    auto reg_upper = this->getInsReg(insn, REG_UPPER);
    auto reg_lower = this->getInsReg(insn, REG_LOWER);

    llvm::Value* isLt = this->builder.CreateICmpULT(reg_upper, reg_lower);
    llvm::Value* isGt = this->builder.CreateICmpUGT(reg_upper, reg_lower);

    llvm::Value* cmpFlag = this->builder.getInt32(0);
    cmpFlag = this->builder.CreateSelect(isLt, this->builder.getInt32(-1), cmpFlag);
    cmpFlag = this->builder.CreateSelect(isGt, this->builder.getInt32(1),  cmpFlag);

    auto reg_cmp_ptr = this->getReg(REG_CMP, true);
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(cmpFlag, reg_cmp_ptr));   
}

void VmHandlers::opRet(Instruction insn)
{
    auto ret_reg_ptr = this->getReg(REG_1, true);
    llvm::Value* result = this->builder.CreateLoad(this->builder.getInt32Ty(), ret_reg_ptr);
    this->builder.CreateRet(result);
}

void VmHandlers::opGetUserData(Instruction insn)
{
    auto reg_ptr = this->getInsRegPtr(insn, REG_UPPER);
    llvm::Value* off = this->loadBuffpOffset();
    llvm::LoadInst* val = this->builder.CreateLoad(this->builder.getInt8Ty(), offsetToPtr(off, this->buff));
    _LLVM_ALLIG_MEM_PAD_SIZE(val);
    llvm::Value* val32 = this->builder.CreateZExt(val, this->builder.getInt32Ty());
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(val32, reg_ptr));
}

void VmHandlers::opSetUserData(Instruction insn)
{
    auto reg = this->getInsReg(insn, REG_UPPER);
    llvm::Value* off = this->loadBuffpOffset();
    llvm::Value* val8 = this->builder.CreateTrunc(reg, this->builder.getInt8Ty());
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(val8, offsetToPtr(off, this->buff)));
}

void VmHandlers::opIncUserData(Instruction insn)
{
    llvm::Value* off = this->loadBuffpOffset();
    llvm::Value* new_off = this->builder.CreateAdd(off, this->builder.getInt32(1));
    storeBuffpOffset(new_off);
}

void VmHandlers::opDecUserData(Instruction insn)
{
    llvm::Value* off = this->loadBuffpOffset();
    llvm::Value* new_off = this->builder.CreateSub(off, this->builder.getInt32(1));
    storeBuffpOffset(new_off);
}

void VmHandlers::opLoop(Instruction insn)
{
    uint64_t target_addr = insn.operands[0];
    auto tgt_it = this->addr_to_bb.find(target_addr);
    
    llvm::BasicBlock* target_bb = tgt_it->second;
            
    uint64_t ft_addr = insn.address + insn.size;
    auto ft_it = this->addr_to_bb.find(ft_addr);
    
    llvm::BasicBlock* fallthrough_bb = ft_it->second;

    llvm::Value* reg_loop = this->getReg(REG_LOOP, false);
    llvm::Value* reg_loop_ptr = this->getReg(REG_LOOP, true);

    auto sub_result = this->builder.CreateSub(reg_loop, this->builder.getInt32(1));
    _LLVM_ALLIG_MEM_PAD_SIZE(this->builder.CreateStore(sub_result, reg_loop_ptr));

    auto cmp_result = this->builder.CreateICmpSGE(sub_result, this->builder.getInt32(0));
    auto *br = this->builder.CreateCondBr(cmp_result, target_bb, fallthrough_bb);
}

void VmHandlers::opJx(Instruction insn, int32_t cmp_val)
{
    uint64_t target_addr = insn.operands[0];
    auto tgt_it = this->addr_to_bb.find(target_addr);
    if (tgt_it == this->addr_to_bb.end())
    {    
        __assume(false);
    }

    llvm::BasicBlock* target_bb = tgt_it->second;
    if (!target_bb)
    {
        __assume(false);
    }

    uint64_t ft_addr = insn.address + insn.size;
    auto ft_it = this->addr_to_bb.find(ft_addr);
    if (ft_it == this->addr_to_bb.end())
    {    
        __assume(false);
    }
    
    llvm::BasicBlock* fallthrough_bb = ft_it->second;

    llvm::Value* reg_cmp = this->getReg(REG_CMP, false);
    llvm::Value* result = this->builder.CreateICmpEQ(reg_cmp, this->builder.getInt32(cmp_val));
    this->builder.CreateCondBr(result, target_bb, fallthrough_bb);
}

void VmHandlers::opJl(Instruction insn)
{
    this->opJx(insn, -1);
}

void VmHandlers::opJg(Instruction insn)
{
    this->opJx(insn, 1);
}

void VmHandlers::opJe(Instruction insn)
{
    this->opJx(insn, 0);
}

void VmHandlers::opModifyNext16Instructions(Instruction insn)
{
    return;
}

VmHandlers::HandlerFn VmHandlers::getHandler(Instruction insn)
{
    switch (insn.opcode)
    {
        case OpCode::RET:
            return &VmHandlers::opRet;
        
        case OpCode::SUB:
            return &VmHandlers::opSub;

        case OpCode::ADD:    
            return &VmHandlers::opAdd;

        case OpCode::MUL:
            return &VmHandlers::opMul;

        case OpCode::DIV:
            return &VmHandlers::opDiv;

        case OpCode::XOR:
            return &VmHandlers::opXor;

        case OpCode::AND:
            return &VmHandlers::opAnd;

        case OpCode::MOV:
            return &VmHandlers::opMov;

        case OpCode::CMP:
            return &VmHandlers::opCmp;
        
        case OpCode::INC:
            return &VmHandlers::opInc;

        case OpCode::DEC:
            return &VmHandlers::opDec;
        
        case OpCode::PUSHREG:
            return &VmHandlers::opPushReg;
        
        case OpCode::SETUSERDATA:
            return &VmHandlers::opSetUserData;

        case OpCode::GETUSERDATA:
            return &VmHandlers::opGetUserData;

        case OpCode::POP:
            return &VmHandlers::opPop;

        case OpCode::PUSHDATA:
            return &VmHandlers::opPushData;
        
        case OpCode::LOOP:
            return &VmHandlers::opLoop;

        case OpCode::JL:
            return &VmHandlers::opJl;

        case OpCode::JG:
            return &VmHandlers::opJg;

        case OpCode::JE:
            return &VmHandlers::opJe;

        case OpCode::DECUSERDATA:
            return &VmHandlers::opDecUserData;

        case OpCode::INCUSERDATA:
            return &VmHandlers::opIncUserData;

        case OpCode::MODIFYNEXT16INSTRUCTIONS:
            return &VmHandlers::opModifyNext16Instructions;
    }

    __assume(false);
}