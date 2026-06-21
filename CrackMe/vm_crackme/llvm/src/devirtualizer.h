#pragma once
#include <iostream>

#include "disassembler.h"
#include "lifter.h"


class Devirtualizer
{
    private:
        Lifter          lifter;
        Disassembler    disassembler;

    public:
        Devirtualizer(const std::vector<uint8_t> vm_code);
        void devirtualize(char *out_ll_filename);
};