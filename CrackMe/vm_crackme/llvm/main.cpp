#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "src/vm_info.h"
#include "src/devirtualizer.h"


void main(int argc, char* argv[])
{
    if (argc != 3) 
    {
        std::cerr << "Usage: " << argv[0] << " <vm_code> <out_file.ll>";
        return;
    }
 
    std::ifstream file(argv[1], std::ios::binary);
    std::vector<uint8_t> vm_code(std::istreambuf_iterator<char>(file), {});

    auto devirtualizer = Devirtualizer(vm_code);
    devirtualizer.devirtualize(argv[2]);
}