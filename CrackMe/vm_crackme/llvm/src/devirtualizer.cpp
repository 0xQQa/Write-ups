#include "devirtualizer.h"


Devirtualizer::Devirtualizer(const std::vector<uint8_t> vm_code_) : disassembler(vm_code_), lifter() 
{
}

void Devirtualizer::devirtualize(char *out_ll_filename)
{
    auto insns = this->disassembler.getAllInstructions();
    printf("Got %u instruciotns ...\n", insns.size());
    if (insns.size() == 0)
    {
        return;
    }

    printf("Parsing VM code to LLVM code ...\n");
    this->lifter.parseVmCode(insns);
    
    printf("Saving LLVM code ...\n");
    this->lifter.saveOutput(out_ll_filename);

    printf("Done! Use clang to optimize!\n");
}