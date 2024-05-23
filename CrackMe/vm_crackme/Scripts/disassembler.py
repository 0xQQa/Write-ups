from inspect import stack
from struct import unpack


class Register:

	def __init__(self) -> None:
		self.REGS = list(map(lambda x: f"reg_{x}", range(1,5)))

	def __get_reg(self, reg_idx) -> str:
		return self.REGS[reg_idx] if reg_idx < 5 else "0"

	def get_reg_upper(self, ins: int) -> str:
		return ins >> 4

	def get_reg_lower(self, ins: int) -> str:
		return ins & 0xF

	def get(self, ins: int) -> str:
		return self.__get_reg(self.get_reg_upper(ins))

	def gets(self, ins: int, get_one: bool) -> str:
		reg_1 = self.__get_reg(self.get_reg_upper(ins))
		if get_one: return f" {reg_1}"
	
		reg_2 = self.__get_reg(self.get_reg_lower(ins))
		return f" {reg_1}, {reg_2}"


class Instructions:

	def __init__(self) -> None:
		self.REGS = Register()
		self.OP_CODES = (self.__end, self.__add, self.__sub, self.__mul, self.__div, self.__inc, self.__dec, self.__xor, self.__and, self.__push_reg, self.__push_data, self.__pop, self.__mov, self.__get_usr_data, self.__set_usr_data, self.__loop, self.__cmp, self.__jl, self.__jg, self.__je, self.__inc_usr_data, self.__dec_usr_data, self.__modify_next_16_insn)

	def __common_ops(self, insn: list, pc: int, get_one: bool = False) -> tuple[int, str]:
		return 2, stack()[1][3].lower()[2:] + self.REGS.gets(insn[pc+1], get_one)

	def __common_jmp(self, ins: int) -> tuple[int, str]:
		return 2, stack()[1][3].lower()[2:] + f" 0x{ins + 2:03x}"

	def __add(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __sub(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __mul(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __div(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)
	
	def __inc(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __dec(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __xor(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __and(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __push_reg(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __push_data(self, insn:list, pc:int) -> tuple[int, str]:
		data = hex(unpack(">I", bytes(insn[pc+1: pc+5]))[0])
		return 5, f"push_data {data}"

	def __pop(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __mov(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __get_usr_data(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __set_usr_data(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc, True)

	def __loop(self, insn:list, pc:int) -> tuple[int, str]:
		loop_to = f"{pc-insn[pc+1]:03x}"
		return 2, f"loop {loop_to}"

	def __cmp(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_ops(insn, pc)

	def __jl(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_jmp(pc+insn[pc+1])

	def __jg(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_jmp(pc+insn[pc+1])

	def __je(self, insn:list, pc:int) -> tuple[int, str]:
		return self.__common_jmp(pc+insn[pc+1])

	def __inc_usr_data(self, *_: list) -> tuple[int, str]:
		return 1, f"inc usr_data"

	def __dec_usr_data(self, *_: list) -> tuple[int, str]:
		return 1, f"dec usr_data"

	def __inc_pc(self, *_: list) -> tuple[int, str]:
		return 1, "nop"

	def __modify_next_16_insn(self, insn:list, pc:int =None) -> tuple[int, str]:
		for idx in range(1, 16): insn[pc + idx] ^= 0x66
		return 1, "nop "

	def __end(self, *_: list) -> tuple[int, str]:
		return 1, "ret"

	def gets(self, insn: list, pc: int) -> tuple[int, str]:
		op_code = insn[pc] - 0x66
		if not (0x00 <= op_code <= 0x16):
			tmp_pc, dis_line = self.__inc_pc(insn[pc + 1])
		else:
			tmp_pc, dis_line = self.OP_CODES[op_code](insn, pc)

		return tmp_pc, f"0x{pc:03x}: {dis_line}"


class Disassembler:

	def __init__(self) -> None:
		self.INSCTRUCTIONS = Instructions()
		self.CODE = [0xde, 0xad, 0xc0, 0xde, 0x7c, 0x2e, 0x27, 0x25, 0x2d, 0x39, 0x32, 0x2e, 0x23, 0x39, 0x36, 0x2a, 0x27, 0x28, 0x23, 0x32, 0x70, 0x0, 0x0, 0x0, 0x2f, 0x75, 0x5, 0x71, 0x30, 0x73, 0x0, 0x6d, 0x22, 0x76, 0x2, 0x79, 0x33, 0x7a, 0x70, 0x0, 0x0, 0x0, 0x46, 0x71, 0x10, 0x76, 0x1, 0x78, 0x27, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x10, 0x76, 0x1, 0x77, 0x16, 0x70, 0x0, 0x0, 0x0, 0x39, 0x71, 0x10, 0x76, 0x1, 0x77, 0xb, 0x70, 0x0, 0x0, 0x0, 0x41, 0x71, 0x1, 0x76, 0x1, 0x77, 0x6, 0x6d, 0x0, 0x76, 0x0, 0x79, 0x5, 0x6d, 0x0, 0x6b, 0x0, 0x66, 0x75, 0x40, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x70, 0xf3, 0x37, 0x46, 0xe6, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x3, 0x6b, 0x0, 0x66, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x70, 0x54, 0x96, 0x27, 0x66, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x3, 0x6b, 0x0, 0x66, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x6c, 0x10, 0x70, 0x2, 0x54, 0x26, 0x1, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x3, 0x6b, 0x0, 0x66, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x6b, 0x10, 0x70, 0x54, 0x77, 0x2, 0xe7, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x3, 0x6b, 0x0, 0x66, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x70, 0x16, 0x36, 0xc2, 0xf6, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x3, 0x6b, 0x0, 0x66, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x30, 0x6d, 0x11, 0x7b, 0x73, 0x0, 0x70, 0x0, 0x0, 0x0, 0x30, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0xa, 0x71, 0x20, 0x76, 0x2, 0x77, 0x9, 0x70, 0x0, 0x0, 0x0, 0x7, 0x71, 0x20, 0x68, 0x2, 0x70, 0x0, 0x0, 0x0, 0x10, 0x71, 0x20, 0x69, 0x12, 0x67, 0x10, 0x75, 0x2b, 0x70, 0x16, 0x86, 0x57, 0x47, 0x71, 0x20, 0x76, 0x12, 0x6d, 0x0, 0x79, 0x2, 0x6b, 0x0, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0]
		self.CODE_LEN = len(self.CODE)
		self.PC = 0

	def gets(self) -> str:
		deasembled = ""
		while self.PC + 1 < self.CODE_LEN:
			next_pc_step, curr_line = self.INSCTRUCTIONS.gets(self.CODE, self.PC)
			self.PC += next_pc_step
			deasembled += f"{curr_line}\n"

		return deasembled


if __name__ == "__main__":
	devirt = Disassembler()
	data = devirt.gets()
	with open("deasembled.txt", "w") as f: f.write(data)
