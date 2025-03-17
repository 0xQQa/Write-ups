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

    def negate_inject(Triton: TritonContext) -> None:
        Triton.setConcreteRegisterValue(Triton.registers.rip, pc + instruction.getSize())

        last = Triton.getPathConstraints()[-1]
        Triton.popPathConstraint()
        not_taken = last.getBranchConstraints()[1]["constraint"]
        Triton.pushPathConstraint(not_taken)

        old_zf = Triton.getConcreteRegisterValue(Triton.registers.zf)
        new_zf = 1 - old_zf
        Triton.setConcreteRegisterValue(Triton.registers.zf, new_zf)
        Triton.concretizeRegister(Triton.registers.zf)

    pc = entrypoint
    while entrypoint <= pc <= max_pc:
        opcode = Triton.getConcreteMemoryAreaValue(pc, 16)

        instruction = Instruction()
        instruction.setOpcode(opcode)
        instruction.setAddress(pc)
        Triton.processing(instruction)

        if instruction.getType() == OPCODE.X86.CALL:
            skip_call(Triton)

        if instruction.getType() in (OPCODE.X86.JNE, OPCODE.X86.JE) and instruction.isConditionTaken():            
            negate_inject(Triton)

        pc = Triton.getConcreteRegisterValue(Triton.registers.rip)


if __name__ == '__main__':
    with open(EXE_NAME, "rb") as f:
        program = f.read()
    
    logic_start = 0x314c
    logic_end = logic_start + 0x485
    code = program[logic_start: logic_end]
    
    logic_pc_start = logic_start + 0x140010c00
    logic_pc_end = logic_end + 0x140010c00

    Triton = TritonContext()
    Triton.setArchitecture(ARCH.X86_64)
    Triton.setMode(MODE.ALIGNED_MEMORY, True)
    Triton.setMode(MODE.ONLY_ON_SYMBOLIZED, True)
    Triton.setMode(MODE.CONSTANT_FOLDING, True)
    Triton.setMode(MODE.AST_OPTIMIZATIONS, True)
    Triton.setAstRepresentationMode(AST_REPRESENTATION.PYTHON)

    Triton.setConcreteMemoryAreaValue(logic_pc_start, code)
    Triton.setConcreteRegisterValue(Triton.registers.ebp, BASE_RBP)
    Triton.setConcreteRegisterValue(Triton.registers.esp, BASE_RSP)

    flag_len = 9
    flag_addr = 0x140022450

    for idx in range(flag_len):
        Triton.symbolizeMemory(MemoryAccess(flag_addr + idx, CPUSIZE.BYTE))

    emulate(Triton, logic_pc_start, logic_pc_end)
    
    astCtxt = Triton.getAstContext()
    path = Triton.getPathPredicate() 
    unrolled = astCtxt.unroll(path)
    simpled = Triton.simplify(unrolled)
    model = Triton.getModel(simpled)

    flag = str.join("", [chr(v.getValue()) for _, v in sorted(model.items())])
    print(f"Trying: {flag}")

    result = run([EXE_NAME], input=flag, text=True, capture_output=True)
    result = result.stdout.splitlines().pop(-1)
    print(f"Result: {result}")
