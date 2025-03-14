from triton import TritonContext, ARCH, Instruction, MemoryAccess, OPCODE, MODE, AST_REPRESENTATION, CPUSIZE
from subprocess import run


BASE_RBP = 0x80000000
BASE_RSP = 0xA0000000
EXE_NAME = "rev.bin"


def emulate(Triton: TritonContext, entrypoint: int, max_pc: int) -> None:
    def skip_call(Triton: TritonContext) -> None:
        rsp = Triton.getConcreteRegisterValue(Triton.registers.rsp)
        ret_addr = Triton.getConcreteMemoryValue(MemoryAccess(rsp, CPUSIZE.QWORD))
        Triton.setConcreteRegisterValue(Triton.registers.rip, ret_addr)
        Triton.setConcreteRegisterValue(Triton.registers.rsp, rsp + CPUSIZE.QWORD)

    pc = entrypoint
    while entrypoint <= pc <= max_pc:
        opcode = Triton.getConcreteMemoryAreaValue(pc, 16)

        instruction = Instruction()
        instruction.setOpcode(opcode)
        instruction.setAddress(pc)
        Triton.processing(instruction)

        if instruction.getType() == OPCODE.X86.CALL:
            skip_call(Triton)

        pc = Triton.getConcreteRegisterValue(Triton.registers.rip)


if __name__ == '__main__':
    with open(EXE_NAME, "rb") as f:
        program = f.read()
    
    logic_start = 0x314c
    logic_end = logic_start + 0x48f
    code = program[logic_start: logic_end]
    
    logic_pc_start = logic_start + 0x140010c00
    logic_pc_end = logic_end + 0x140010c00

    Triton = TritonContext()
    Triton.setArchitecture(ARCH.X86_64)
    Triton.setMode(MODE.ALIGNED_MEMORY, True)
    Triton.setAstRepresentationMode(AST_REPRESENTATION.PYTHON)

    Triton.setConcreteMemoryAreaValue(logic_pc_start, code)
    Triton.setConcreteRegisterValue(Triton.registers.ebp, BASE_RBP)
    Triton.setConcreteRegisterValue(Triton.registers.esp, BASE_RSP)

    flag_len = 9
    flag_addr = 0x140022450
    flag_checks = {0x4: 0x15b, 0x24: 0x68, 0x44: 0x1c2, 0x64: 0x57, 0x84: 0x10e, 0xa4: 0x5f, 0xe4:3, 0x104: 0xeb}

    for idx in range(flag_len):
        Triton.symbolizeMemory(MemoryAccess(flag_addr + idx, CPUSIZE.BYTE))

    emulate(Triton, logic_pc_start, logic_pc_end)
    
    constrains = []
    astCtxt = Triton.getAstContext()
    for idx in range(flag_len):
        symv_var = Triton.getSymbolicVariable(f'SymVar_{idx}')
        constrains += [astCtxt.variable(symv_var) > astCtxt.bv(0x20,  CPUSIZE.BYTE_BIT)]
        constrains += [astCtxt.variable(symv_var) < astCtxt.bv(0x7f, CPUSIZE.BYTE_BIT)]
    
    for var_off, var_val in flag_checks.items():
        out = Triton.getSymbolicMemory(BASE_RBP + var_off)
        unrolled = astCtxt.unroll(out.getAst())
        simpled = Triton.simplify(unrolled)
        constrains += [simpled == var_val] 
        
    constrains += [astCtxt.variable(Triton.getSymbolicVariable(f'SymVar_6')) == astCtxt.variable(Triton.getSymbolicVariable(f'SymVar_7'))]
    constrains = astCtxt.land(constrains)
    model = Triton.getModel(constrains)
    flag = str.join("", [chr(v.getValue()) for _, v in sorted(model.items())])
    print(f"Trying: {flag}")
    
    result = run([EXE_NAME], input=flag, text=True, capture_output=True)
    result = result.stdout.splitlines().pop(-1)
    print(f"Result: {result}")