#pragma once

#include "../meow/core.hpp"
#include "../ir/ir.hpp"

namespace buaac {

	enum Reg {
		at,
		v0, v1,
		a0, a1, a2, a3,
		t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
		s0, s1, s2, s3, s4, s5, s6, s7,
		k0, k1,
		gp, sp, fp,
		ra
	};
	
	class RegPool {
	public:
		
	};

	class MIPS {

#define write(...)	\
	fout << FORMAT(__VA_ARGS__) << std::endl

		IR ir;

		std::ofstream fout;

	public:
		MIPS(IR ir): ir(ir) {
			fout.open("mips.txt");
		}

		void gen() {
			write("# Compiler BUAAC, Build: {}, QAQ", __TIMESTAMP__);
			write(".data");
			genGlobal();
			genFunc();
			write(".text");
			genText();
		}

		void genGlobal() {
			auto& defines = ir.global_defines;
			for (int i = 0; i < defines.size(); i++) {
				genDefine(defines[i]);

			}
		}

		void genDefine(GlobalDefine& define) {
			switch (define.type) {

			case GlobalDefine::CONST_STR:
				write("{}: .asciiz \"{}\" ", define.const_str.label, define.const_str.value);
				break;
			case GlobalDefine::VAR_INT:
				write("{}: .space 4", define.var_int.label);
				break;
			default: ;
			}
		}

		void genFunc() {

		}

		void genText() {
			auto& blocks = ir.blocks;
			for (int i = 0; i < blocks.size(); i++) {
				genBlock(blocks[i]);
			}
		}

		void genBlock(Block& block) {
			assignRegs(block);
			genInstrs(block);
		}

		void assignRegs(Block& block) {
			
		}

		void genInstrs(Block& block) {
			write("{}:", block.label);
			auto& instrs = block.instrs;
			for (int i = 0; i < instrs.size(); i++) {
				genInstr(instrs[i]);
			}
		}

		void genInstr(Instr instr) {
			switch (instr.type) {

			case Instr::PRINT_GLOBAL_STR:
				getRegisters({ a0, v0 });
				write("la $a0, {}", instr.target);
				write("li $v0, 4");
				write("syscall");
				releaseRegisters({ a0, v0 });
				break;
			case Instr::PRINT_INT:
				getRegisters({ a0, v0 });
				write("move $a0, {}", instr.target);
				write("li $v0, 1");
				write("syscall");
				releaseRegisters({ a0, v0 });
				break;
			case Instr::PRINT_LINE:
				getRegisters({ a0, v0 });
				write("la $a0, '\\n'");
				write("li $v0, 11");
				write("syscall");
				releaseRegisters({ a0, v0 });
				break;
			case Instr::SCAN_GLOBAL_INT:
				getRegisters({ v0 });
				write("li $v0, 5");
				write("syscall");
				write("sw $v0, {}", instr.target);
				releaseRegisters({ v0 });
				break;
			case Instr::PLUS:
				write("addu {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MINUS:
				write("subu {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MULT:
				write("mul {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::DIV:
				write("divu {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::LOAD_LAB:
				write("lw {}, {}", instr.target, instr.source_a);
				break;
			case Instr::SCAN_INT: break;
			case Instr::PUSH:
				write("subu $sp, {}", instr.target);
				break;
			case Instr::POP:
				write("addu $sp, {}", instr.target);
				break;
			case Instr::LOAD_STA:
				write("lw {}, $sp({})", instr.target, instr.source_a);
				break;
			case Instr::SAVE_LAB:
				write("sw {}, {}", instr.target, instr.source_a);
				break;
			case Instr::SAVE_STA:
				write("sw {}, $sp({})", instr.target, instr.source_a);
				break;
			default: ;


			}
		}

		void getRegisters(std::vector<Reg> regs) {
			// TODO: important
		}

		void releaseRegisters(std::vector<Reg> regs) {
			// TODO: important
		}

		

	};

}
