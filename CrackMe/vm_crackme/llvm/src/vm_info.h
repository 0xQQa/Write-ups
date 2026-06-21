#pragma once

#include <iostream>
#include <vector>


#define VM_MODULE_NAME          "vm_crackme_module"

#define REGS_COUNT              0x5
#define REG_1                   0x0
#define REG_2                   0x1
#define REG_3                   0x2
#define REG_LOOP                0x3
#define REG_CMP                 0x4

#define MEM_PAD_SIZE            0x4
#define STACK_SIZE              0x100  
#define STACK_HEADROOM          0x20  
#define USER_BUFF_SIZE          0x40
   
#define DEAD_CODE_START_SIZE    4 
