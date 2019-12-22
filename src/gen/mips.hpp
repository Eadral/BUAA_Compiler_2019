#pragma once

#include "../meow/core.hpp"
#include "../ir/ir.hpp"
#include "reg_pool.hpp"

namespace buaac {

// #define DO_NOT_ASSIGN_REGS

	
	

	class MIPS {

#define write(...)	\
	fout << FORMAT(__VA_ARGS__) << std::endl

		IR ir;

		std::ofstream fout;

		RegPool reg_pool_;
		
	public:
		MIPS(IR ir): ir(ir) {
			fout.open("mips.txt");
		}

		void gen() {
			write("# Compiler BUAAC, Build: {}, QAQ", __TIMESTAMP__);
			write(".data");
			genGlobal();
			// genRegPool();
			write(".text");
			write("j main");
			genFuncs();
			// genText();
		}

		void genGlobal() {
			auto& defines = ir.global_defines;
			for (int i = 0; i < defines.size(); i++) {
				genDefine(defines[i]);

			}
		}

#define REGPOOL_START 2000 * 4
		// #define REGPOOL "REGPOOL"
// #define POOLSIZE 100
		
		// void genRegPool() {
		// 	write("{}: \n .align 3 \n .space {}", REGPOOL, POOLSIZE*4);
		// }

		void genDefine(GlobalDefine& define) {
			switch (define.type) {

			case GlobalDefine::CONST_STR:
				write("{}: .asciiz \"{}\" ", define.const_str.label, define.const_str.value);
				break;
			// case GlobalDefine::VAR_INT:
			// 	write("{}: .space 4", define.var_int.label);
			// 	break;
			// case GlobalDefine::VAR_INT_ARR:
			// 	write("{}: .space {}", define.var_int_arr.label, 4*define.var_int_arr.len);
			// 	break;
			default: ;
			}
		}

		void genFuncs() {
			auto& funcs = ir.funcs;
			for (int i = 0; i < funcs.size(); i++) {
				reg_pool_.resetRegPool();
				genFunc(funcs[i]);
			}
			// main Func
			// resetRegPool();
			// auto& blocks = ir.getMainBlocks();
			// for (int i = 0; i < blocks.size(); i++) {
			// 	genBlock(blocks[i]);
			// }
		}
		
		

#define REGPOOL_LOAD(ir_reg, assign_reg)	\
		do {	\
			auto& instr = instrs[k];	\
			if (starts_with(instr.ir_reg, string("__T"))) {	\
				int loc = 4 * reg_pool_.getMemPool(instr.ir_reg.substr(3));	\
				instr.ir_reg = #assign_reg;	\
				insertBefore(instrs, k, Instr(Instr::LOAD_GLO, #assign_reg, REGPOOL_START+loc));	\
			}	\
		} while(0)

#define REGPOOL_SAVE(ir_reg, assign_reg)	\
		do {	\
			auto& instr = instrs[k];	\
			if (starts_with(instr.ir_reg, string("__T"))) {	\
				int loc = 4 * reg_pool_.getMemPool(instr.ir_reg.substr(3));	\
				instr.ir_reg = #assign_reg;	\
				insertAfter(instrs, k, Instr(Instr::SAVE_GLO, #assign_reg, REGPOOL_START+loc));	\
			}	\
		} while(0)

#define NOT_ASSIGN(ir_reg) starts_with(instrs[k].ir_reg, string("__T"))


#define assignReg(ir_reg)	\
		do {	\
			if (starts_with(ir_reg, string("__T"))) {	\
				ir_reg = reg_pool_.getRegPool(ir_reg);	\
			} \
		} while(0)
	
		
		// void assignRegs(Block& block) {
		// 	
		// }

		vector<string> ts;
		vector<int> pushPoolSize;
		
		void pushRegPool(int size) {
			pushPoolSize.push_back(size);
			for (int i = 0; i < size; i++) {
				write("lw {}, {}($gp)", "$k0", REGPOOL_START + 4 * i);
				write("sw {}, ($sp)", "$k0");
				write("addi $sp, $sp, -4");
			}
		}
		

		void popRegPool() {
			for (int i = pushPoolSize.back()-1; i >= 0; i--) {
				write("addi $sp, $sp, 4");
				write("lw {}, ($sp)", "$k0");
				write("sw {}, {}($gp)", "$k0", REGPOOL_START+4 * i);
			}
			pushPoolSize.pop_back();
		}

		void insertBefore(vector<Instr> &instrs, int &i, Instr instr) {
			instrs.insert(instrs.begin() + i, instr);
			i++;
		}

		void insertAfter(vector<Instr> &instrs, int &i, Instr instr) {
			instrs.insert(instrs.begin() + i + 1, instr);
		}

		void genInstrs(Block& block) {
			write("{}:", block.label);
			auto& instrs = block.instrs;
			for (int i = 0; i < instrs.size(); i++) {
				genInstr(instrs[i]);
			}
		}

		bool isNumber(string source) {
			return isdigit(source[0]) || source[0] == '-';
		}

		void assignRegister(string &reg) {
			if (starts_with(reg, string("__T"))) {
				reg = reg_pool_.getAndAllocRegPool(reg);
			}
		}

		void getRegister(string &reg) {
			if (starts_with(reg, string("__T"))) {
				reg = reg_pool_.getRegPool(reg);
			}
		}

		void genFunc(Func& func) {

			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)

				if (instr.type == Instr::IR_SHOW)
					continue;
				// for (int i = 0; i < instrs.size(); i++) {

					// auto& instr = instrs[i];
					// if (starts_with(instrs[k].target, string("__T"))) {
					// 	instrs[k].target = reg_pool_.getRegPool(instrs[k].target);
					// }
				if (instr.block_line_number != -1) {
					reg_pool_.checkAndRelease(ir.func_to_ident_to_range[func.func_name], j, instr.block_line_number);
				}

				if (instrs[k].targetIsSave()) {
					assignRegister(instrs[k].target);
				} else {
					getRegister(instrs[k].target);
				}

				if (instrs[k].sourceAIsSave()) {
					assignRegister(instrs[k].source_a);
				}
				else {
					getRegister(instrs[k].source_a);
				}

				if (instrs[k].sourceBIsSave()) {
					assignRegister(instrs[k].source_b);
				}
				else {
					getRegister(instrs[k].source_b);
				}
			

				if (NOT_ASSIGN(target)) {
					// panic("not assign");
					if (instrs[k].targetIsLoad())
						REGPOOL_LOAD(target, $a0);
					if (instrs[k].targetIsSave())
						REGPOOL_SAVE(target, $a0);
				}
				if (NOT_ASSIGN(source_a)) {
					// panic("not assign");
					if (instrs[k].sourceAIsLoad())
						REGPOOL_LOAD(source_a, $a1);
					if (instrs[k].sourceAIsSave())
						REGPOOL_SAVE(source_a, $a1);
				}
				if (NOT_ASSIGN(source_b)) {
					// panic("not assign");
					if (instrs[k].sourceBIsLoad())
						REGPOOL_LOAD(source_b, $a2);
					if (instrs[k].sourceBIsSave())
						REGPOOL_SAVE(source_b, $a2);
				}

				if (instrs[k].type == Instr::PUSH_REGPOOL) {

					// case Instr::PUSH_REGPOOL:
					int push_number = a2i(instr.target);

					ts.clear();
					for (int x = 0; x < reg_pool_.globalRegs.size(); x++) {
						if (reg_pool_.availReg[reg_pool_.globalRegs[x]] == false) {
							ts.push_back(reg_pool_.globalRegs[x]);
						}
					}
#ifdef STANDARD_REG_A
					ts.push_back("$a0");
					ts.push_back("$a1");
					ts.push_back("$a2");
					ts.push_back("$a3");
#endif
					reg_pool_.t_stack.push_back(ts);
					for (int y = 0; y < ts.size(); y++) {
						insertAfter(instrs, k, { Instr::PUSH_REG, ts[y] });
						push_number++;
					}
					instrs[k].source_b = i2a(reg_pool_.mempool_size);
					push_number += reg_pool_.mempool_size;
					// instrs.insert(instrs.begin() + k + 1 + ts.size(), Instr(Instr::PLUS, "$fp", "$sp", int(push_number*4)));
				}
				else if (instrs[k].type == Instr::POP_REGPOOL) {
					ts = reg_pool_.t_stack.back();
					reg_pool_.t_stack.pop_back();
					for (int y = 0; y < ts.size(); y++) {
						insertBefore(instrs, k, { Instr::POP_REG, ts[y] });
					}
				}

				EndFor
			// }

			
			genInstrs(block);


			EndFor


		}
		
		void genInstr(Instr instr) {
			switch (instr.type) {

			case Instr::PRINT_GLOBAL_STR:
				getRegisters({ "$a0", "$v0" });
				write("la $a0, {}", instr.target);
				write("li $v0, 4");
				write("syscall");
				releaseRegisters({ "$a0", "$v0"});
				break;
			
			case Instr::PRINT_LINE:
				getRegisters({ "$a0", "$v0" });
				write("la $a0, '\\n'");
				write("li $v0, 11");
				write("syscall");
				releaseRegisters({ "$a0", "$v0" });
				break;
			
			case Instr::PLUS:
				if (isNumber(instr.source_b)) {
					write("addi {}, {}, {}", instr.target, instr.source_a, instr.source_b);
					break;
				}
				if (isNumber(instr.source_a)) {
					write("addi {}, {}, {}", instr.target, instr.source_b, instr.source_a);
					break;
				}
				write("addu {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MINUS:
				if (isNumber(instr.source_b)) {
					write("subi {}, {}, {}", instr.target, instr.source_a, instr.source_b);
					break;
				}
				if (isNumber(instr.source_a)) {
					write("li $k0, {}", instr.source_a);
					instr.source_a = "$k0";
					write("sub {}, {}, {}", instr.target, instr.source_a, instr.source_b);
					break;
				}
				write("subu {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MULT:
				if (isNumber(instr.source_b)) {
					write("li $k0, {}", instr.source_b);
					instr.source_b = "$k0";
				}
				if (isNumber(instr.source_a)) {
					write("li $k0, {}", instr.source_a);
					instr.source_a = "$k0";
				}
				write("mul {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::DIV:
				if (isNumber(instr.source_a)) {
					write("li $k0, {}", instr.source_a);
					instr.source_a = "$k0";
				}
				if (isNumber(instr.source_b) && ispow2(a2i(instr.source_b)) ) {
					write("srl {}, {}, {}", instr.target, instr.source_a, static_cast<int>(std::log2(a2i(instr.source_b))));
					break;
				}
				if (isNumber(instr.source_b)) {
					write("li $k0, {}", instr.source_b);
					instr.source_b = "$k0";
				}
				
				write("div {}, {}", instr.source_a, instr.source_b);
				write("mflo {}", instr.target);
				break;
			// case Instr::LOAD_LAB:
			// 	write("lw {}, {}", instr.target, instr.source_a);
			// 	break;
			
			case Instr::PUSH:
				write("addi $sp, $sp, -{}", instr.target);
				break;
			case Instr::POP:
				write("addi $sp, $sp, {}", instr.target);
				break;
			case Instr::LOAD_STA:
				write("lw {}, {}($fp)", instr.target, instr.source_a);
				break;
			case Instr::SAVE_STA:
				if (isNumber(instr.target)) {
					write("li $k0, {}", instr.target);
					instr.target = "$k0";
				}
				write("sw {}, {}($fp)", instr.target, instr.source_a);
				break;
			case Instr::LOAD_GLO:
				write("lw {}, {}($gp)", instr.target, instr.source_a);
				break;
			case Instr::SAVE_GLO:
				if (isNumber(instr.target)) {
					write("li $k0, {}", instr.target);
					instr.target = "$k0";
				}
				write("sw {}, {}($gp)", instr.target, instr.source_a);
				break;
			// case Instr::LOAD_LAB_IMM:
			// 	write("lw {}, {}+{}", instr.target, instr.source_a, instr.source_b);
			// 	break;
			// case Instr::SAVE_LAB_IMM:
			// 	write("sw {}, {}+{}", instr.target, instr.source_a, instr.source_b);
			// 	break;
			case Instr::PUSH_REG:
				if (isNumber(instr.target)) {
					write("li $k0, {}", instr.target);
					instr.target = "$k0";
				}
				write("sw {}, ($sp)", instr.target);
				write("addi $sp, $sp, -4");
				break;
			case Instr::POP_REG:
				write("addi $sp, $sp, 4");
				write("lw {}, ($sp)", instr.target);
				break;
			case Instr::CALL:
				write("jal {}", instr.target);
				break;
			case Instr::RETURN_END:
				write("jr $ra");
				break;
			case Instr::JUMP:
				write("j {}", instr.target);
				break;
			case Instr::BGEZ:
				write("bgez {}, {}", instr.target, instr.source_a);
				break;
			case Instr::BLEZ:
				write("blez {}, {}", instr.target, instr.source_a);
				break;
			case Instr::BGTZ:
				write("bgtz {}, {}", instr.target, instr.source_a);
				break;
			case Instr::BLT:
				if (isNumber(instr.target)) {
					write("bgt {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("blt {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGT:
				if (isNumber(instr.target)) {
					write("blt {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("bgt {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BLE:
				if (isNumber(instr.target)) {
					write("bge {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("ble {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGE:
				if (isNumber(instr.target)) {
					write("ble {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("bge {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BEQ:
				if (isNumber(instr.target)) {
					write("beq {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("beq {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BNE:
				if (isNumber(instr.target)) {
					write("bne {}, {}, {}", instr.source_a, instr.target, instr.source_b);
					break;
				}
				write("bne {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
				
			case Instr::PRINT_INT:
				getRegisters({ "$a0", "$v0" });
				if (isNumber(instr.target)) {
					write("li $a0, {}", instr.target);
				} else {
					write("move $a0, {}", instr.target);
				}
				write("li $v0, 1");
				write("syscall");
				releaseRegisters({ "$a0", "$v0" });
				break;
			case Instr::SCAN_GLOBAL_INT:
				getRegisters({ "$v0" });
				write("li $v0, 5");
				write("syscall");
				write("sw $v0, {}($gp)", instr.target);
				releaseRegisters({ "$v0" });
				break;
			case Instr::SCAN_INT:
				getRegisters({ "$v0" });
				write("li $v0, 5");
				write("syscall");
				if (starts_with(instr.target, string("$")))
					write("move {}, $v0", instr.target);
				else
					write("sw $v0, {}($fp)", instr.target);
				releaseRegisters({ "$v0" });
				break;
			case Instr::SCAN_GLOBAL_CHAR:
				getRegisters({ "$v0" });
				write("li $v0, 12");
				write("syscall");
				write("sw $v0, {}($gp)", instr.target);
				releaseRegisters({ "$v0" });
				break;
			case Instr::SCAN_CHAR:
				getRegisters({ "$v0" });
				write("li $v0, 12");
				write("syscall");
				if (starts_with(instr.target, string("$")))
					write("move {}, $v0", instr.target);
				else
					write("sw $v0, {}($fp)", instr.target);
				releaseRegisters({ "$v0" });
				break;
			case Instr::PRINT_CHAR:
				getRegisters({ "$a0", "$v0" });
				if (isNumber(instr.target)) {
					write("li $a0, {}", instr.target);
				}
				else {
					write("move $a0, {}", instr.target);
				}
				write("li $v0, 11");
				write("syscall");
				releaseRegisters({ "$a0", "$v0" });
				break;
			

			case Instr::SAVE_ARR_STA:
				if (isNumber(instr.source_b)) {
					write("addi $k0, $fp, -{}", a2i(instr.source_b)*4);
				} else {
					write("sll $k0, {}, 2", instr.source_b);
					write("sub $k0, $fp, $k0");
				}
				if (isNumber(instr.target)) {
					write("li $at, {}", instr.target);
					instr.target = "$at";
				}
				write("sw {}, {}($k0)", instr.target, instr.source_a);
				break;
			case Instr::LOAD_ARR_STA:
				if (isNumber(instr.source_b)) {
					write("addi $k0, $fp, -{}", a2i(instr.source_b) * 4);
				} else {
					write("sll $k0, {}, 2", instr.source_b);
					write("sub $k0, $fp, $k0");
				}
				if (isNumber(instr.target)) {
					write("li $at, {}", instr.target);
					instr.target = "$at";
				}
				write("lw {}, {}($k0)", instr.target, instr.source_a);
				break;
			case Instr::SAVE_ARR_GLO:
				if (isNumber(instr.source_b)) {
					write("addi $k0, $gp, {}", a2i(instr.source_b) * 4);
				} else {
					write("sll $k0, {}, 2", instr.source_b);
					write("add $k0, $k0, $gp");
				}
				if (isNumber(instr.target)) {
					write("li $at, {}", instr.target);
					instr.target = "$at";
				}
				write("sw {}, {}($k0)", instr.target, instr.source_a);
				break;
			case Instr::LOAD_ARR_GLO:
				if (isNumber(instr.source_b)) {
					write("addi $k0, $gp, {}", a2i(instr.source_b) * 4);
				} else {
					write("sll $k0, {}, 2", instr.source_b);
					write("add $k0, $k0, $gp");
				}
				if (isNumber(instr.target)) {
					write("li $at, {}", instr.target);
					instr.target = "$at";
				}
				write("lw {}, {}($k0)", instr.target, instr.source_a);
				break;
				
			case Instr::LA:
				write("la {}, {}", instr.target, instr.source_a);
				break;
			case Instr::MOVE:
				write("move {}, {}", instr.target, instr.source_a);
				break;
			case Instr::LI:
				write("li {}, {}", instr.target, instr.source_a);
				break;
			case Instr::PUSH_REGPOOL:
				pushRegPool(a2i(instr.source_b));
				break;
			case Instr::POP_REGPOOL:
				popRegPool();
				break;
			case Instr::IR_SHOW: break;
			case Instr::NOP: break;
			default: ;


			}
		}

		

		void getRegisters(vector<string> regs) {
#ifdef STANDARD_REG_A
			if (regs.size() == 1) {
				write("move {}, {}", "$k0", regs[0]);
			} else if (regs.size() == 2) {
				write("move {}, {}", "$k0", regs[0]);
				write("move {}, {}", "$k1", regs[1]);
			}
#endif
		}

		void releaseRegisters(vector<string> regs) {
#ifdef STANDARD_REG_A
			if (regs.size() == 1) {
				write("move {}, {}", regs[0], "$k0");
			}
			else if (regs.size() == 2) {
				write("move {}, {}", regs[0], "$k1");
				write("move {}, {}", regs[0], "$k1");
			}
#endif
		}

		

	};

}
