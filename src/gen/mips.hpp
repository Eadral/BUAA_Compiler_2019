#pragma once

#include "../meow/core.hpp"
#include "../ir/ir.hpp"

namespace buaac {

// #define DO_NOT_ASSIGN_REGS
	
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
			genRegPool();
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

		#define REGPOOL "REGPOOL"
#define POOLSIZE 100
		
		void genRegPool() {
			write("{}: \n .align 3 \n .space {}", REGPOOL, POOLSIZE*4);
		}

		void genDefine(GlobalDefine& define) {
			switch (define.type) {

			case GlobalDefine::CONST_STR:
				write("{}: .asciiz \"{}\" ", define.const_str.label, define.const_str.value);
				break;
			case GlobalDefine::VAR_INT:
				write("{}: .space 4", define.var_int.label);
				break;
			case GlobalDefine::VAR_INT_ARR:
				write("{}: .space {}", define.var_int_arr.label, 4*define.var_int_arr.len);
				break;
			default: ;
			}
		}

		void genFuncs() {
			auto& funcs = ir.funcs;
			for (int i = 0; i < funcs.size(); i++) {
				resetRegPool();
				genFunc(funcs[i]);
			}
			// main Func
			// resetRegPool();
			// auto& blocks = ir.getMainBlocks();
			// for (int i = 0; i < blocks.size(); i++) {
			// 	genBlock(blocks[i]);
			// }
		}

		void genFunc(Func &func) {
			auto& blocks = *(func.blocks);
			for (int i = 0; i < blocks.size(); i++) {
				genBlock(blocks[i]);
			}
		}
		
		void genBlock(Block& block) {
			
#ifndef DO_NOT_ASSIGN_REGS
			assignRegs(block);
#endif
			genInstrs(block);
		}

		int mempool_size = 0;
		std::map<std::string, int> memPool;

		std::vector<std::string> globalRegs = {
			"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
			"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
			"$v1"
		};

		std::map<std::string, std::string> regPool;
		std::map<std::string, bool> availReg;
		std::vector<std::vector<std::string>> t_stack;

		void resetRegPool() {
			mempool_size = 0;
			memPool.clear();
			regPool.clear();
			for (int i = 0; i < globalRegs.size(); i++) {
				availReg[globalRegs[i]] = true;
			}
			t_stack.clear();
		}

		int allocMemPool(std::string id) {
			memPool[id] = mempool_size;
			return mempool_size++;
		}

		int getMemPool(std::string id) {
			if (memPool.find(id) == memPool.end()) {
				return allocMemPool(id);
			}
			return memPool[id];
		}

		std::string getReg() {
			for (int i = 0; i < globalRegs.size(); i++) {
				if (availReg[globalRegs[i]]) {
					availReg[globalRegs[i]] = false;
					return globalRegs[i];
				}
			}
			panic("no avail reg");
		}

		bool haveAvailReg() {
			for (int i = 0; i < globalRegs.size(); i++) {
				if (availReg[globalRegs[i]]) {
					return true;
				}
			}
			return false;
		}
		
		void allocRegPool(std::string id) {
			std::string reg = getReg();
			regPool[id] = reg;
		}

		std::string getRegPool(std::string id) {
			if (regPool.find(id) == regPool.end()) {
				if (haveAvailReg()) {
					allocRegPool(id);
				} else {
					return id;
				}
			}
			return regPool[id];
		}

#define REGPOOL_LOAD(ir_reg, assign_reg)	\
		do {	\
			auto& instr = instrs[i];	\
			if (starts_with(instr.ir_reg, std::string("__T"))) {	\
				int loc = 4 * getMemPool(instr.ir_reg.substr(3));	\
				instr.ir_reg = #assign_reg;	\
				insertBefore(instrs, i, Instr(Instr::LOAD_LAB_IMM, #assign_reg, REGPOOL, loc));	\
			}	\
		} while(0)

#define REGPOOL_SAVE(ir_reg, assign_reg)	\
		do {	\
			auto& instr = instrs[i];	\
			if (starts_with(instr.ir_reg, std::string("__T"))) {	\
				int loc = 4 * getMemPool(instr.ir_reg.substr(3));	\
				instr.ir_reg = #assign_reg;	\
				insertAfter(instrs, i, Instr(Instr::SAVE_LAB_IMM, #assign_reg, REGPOOL, loc));	\
			}	\
		} while(0)

#define NOT_ASSIGN(ir_reg) starts_with(instrs[i].ir_reg, std::string("__T"))


#define assignReg(ir_reg)	\
		do {	\
			if (starts_with(ir_reg, std::string("__T"))) {	\
				ir_reg = getRegPool(ir_reg);	\
			} \
		} while(0)
	
		
		void assignRegs(Block& block) {
			auto& instrs = block.instrs;
			for (int i = 0; i < instrs.size(); i++) {
				
				auto& instr = instrs[i];
				
				assignReg(instr.target);
				assignReg(instr.source_a);
				assignReg(instr.source_b);
				
				
				switch (instrs[i].type) {

				case Instr::PRINT_GLOBAL_STR: break;
				case Instr::PRINT_CHAR: 
				case Instr::PRINT_INT:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					break;
				case Instr::SCAN_INT: break;
				case Instr::SCAN_GLOBAL_INT: break;
				case Instr::PRINT_LINE: break;
				
				case Instr::PUSH: break;
				case Instr::POP: break;
				case Instr::LOAD_LAB:
					if (NOT_ASSIGN(target))
						REGPOOL_SAVE(target, $t0);
					break;
				case Instr::LOAD_LAB_IMM: break;
				case Instr::LOAD_STA:
					if (NOT_ASSIGN(target))
						REGPOOL_SAVE(target, $t0);
					break;
				case Instr::SAVE_LAB:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					break;
				case Instr::SAVE_LAB_IMM: break;
				case Instr::SAVE_STA:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					break;

				case Instr::PLUS: 
				case Instr::MINUS: 
				case Instr::MULT:
				case Instr::DIV:
				case Instr::MOVE:
				case Instr::LI:
					if (NOT_ASSIGN(target)) 
						REGPOOL_SAVE(target, $t0);
					if (NOT_ASSIGN(source_a)) 
						REGPOOL_LOAD(source_a, $t1);
					if (NOT_ASSIGN(source_b))
						REGPOOL_LOAD(source_b, $t2);
					break;
				case Instr::PUSH_REG:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					break;
				case Instr::POP_REG: break;
				case Instr::CALL: break;
				case Instr::RETURN_END: break;
				case Instr::JUMP: break;
					
				case Instr::BGEZ: 
				case Instr::BLEZ:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					break;
					
				case Instr::BLT: 
				case Instr::BGT: 
				case Instr::BLE: 
				case Instr::BGE: 
				case Instr::BEQ: 
				case Instr::BNE:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					if (NOT_ASSIGN(source_a))
						REGPOOL_LOAD(source_a, $t1);
					break;
				case Instr::SCAN_CHAR: break;
				case Instr::SCAN_GLOBAL_CHAR: break;
				case Instr::SAVE_STA_ARR:
					if (NOT_ASSIGN(target))
						REGPOOL_LOAD(target, $t0);
					if (NOT_ASSIGN(source_a))
						REGPOOL_LOAD(source_a, $t1);
					break;
				case Instr::LOAD_STA_ARR: 
					if (NOT_ASSIGN(target))
						REGPOOL_SAVE(target, $t0);
					if (NOT_ASSIGN(source_a))
						REGPOOL_LOAD(source_a, $t1);
					break;
				case Instr::LA:
					if (NOT_ASSIGN(target))
						REGPOOL_SAVE(target, $t0);
					break;
				case Instr::PUSH_REGPOOL:
					ts.clear();
					for (int i = 0; i < globalRegs.size(); i++) {
						if (availReg[globalRegs[i]] == false) {
							ts.push_back(globalRegs[i]);
						}
					}
					t_stack.push_back(ts);
					for (int j = 0; j < ts.size(); j++) {
						insertAfter(instrs, i, { Instr::PUSH_REG, ts[j] });
					}
					break;
				case Instr::POP_REGPOOL:
					ts = t_stack.back();
					t_stack.pop_back();
					for (int j = 0; j < ts.size(); j++) {
						insertBefore(instrs, i, { Instr::POP_REG, ts[j] });
					}
					break;
				case Instr::IR_SHOW: break;
				default: ;
				}
				
				
			}
		}

		std::vector<std::string> ts;
		std::vector<int> pushPoolSize;
		
		void pushRegPool() {
			pushPoolSize.push_back(mempool_size);
			for (int i = 0; i < mempool_size; i++) {
				write("lw {}, {}+{}", "$k0", "REGPOOL", 4 * i);
				write("sw {}, ($sp)", "$k0");
				write("addi $sp, $sp, -4");
			}
		}
		

		void popRegPool() {
			for (int i = pushPoolSize.back()-1; i >= 0; i--) {
				write("addi $sp, $sp, 4");
				write("lw {}, ($sp)", "$k0");
				write("sw {}, {}+{}", "$k0", "REGPOOL", 4 * i);
			}
			pushPoolSize.pop_back();
		}

		void insertBefore(std::vector<Instr> &instrs, int &i, Instr instr) {
			instrs.insert(instrs.begin() + i, instr);
			i++;
		}

		void insertAfter(std::vector<Instr> &instrs, int &i, Instr instr) {
			instrs.insert(instrs.begin() + i + 1, instr);
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
			
			case Instr::PRINT_LINE:
				getRegisters({ a0, v0 });
				write("la $a0, '\\n'");
				write("li $v0, 11");
				write("syscall");
				releaseRegisters({ a0, v0 });
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
				write("div {}, {}", instr.source_a, instr.source_b);
				write("mflo {}", instr.target);
				break;
			case Instr::LOAD_LAB:
				write("lw {}, {}", instr.target, instr.source_a);
				break;
			
			case Instr::PUSH:
				write("addi $sp, $sp, -{}", instr.target);
				break;
			case Instr::POP:
				write("addi $sp, $sp, {}", instr.target);
				break;
			case Instr::LOAD_STA:
				write("lw {}, {}($fp)", instr.target, instr.source_a);
				break;
			case Instr::SAVE_LAB:
				write("sw {}, {}", instr.target, instr.source_a);
				break;
			case Instr::SAVE_STA:
				write("sw {}, {}($fp)", instr.target, instr.source_a);
				break;
			case Instr::LOAD_LAB_IMM:
				write("lw {}, {}+{}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::SAVE_LAB_IMM:
				write("sw {}, {}+{}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::PUSH_REG:
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
			case Instr::BLT:
				write("blt {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGT:
				write("bgt {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BLE:
				write("ble {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGE:
				write("bge {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BEQ:
				write("beq {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BNE:
				write("bne {}, {}, {}", instr.target, instr.source_a, instr.source_b);
				break;
				
			case Instr::PRINT_INT:
				getRegisters({ a0, v0 });
				write("move $a0, {}", instr.target);
				write("li $v0, 1");
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
			case Instr::SCAN_INT:
				getRegisters({ v0 });
				write("li $v0, 5");
				write("syscall");
				write("sw $v0, {}($fp)", instr.target);
				releaseRegisters({ v0 });
				break;
				
			case Instr::PRINT_CHAR:
				getRegisters({ a0, v0 });
				write("move $a0, {}", instr.target);
				write("li $v0, 11");
				write("syscall");
				releaseRegisters({ a0, v0 });
				break;
			case Instr::SCAN_GLOBAL_CHAR:
				getRegisters({ v0 });
				write("li $v0, 12");
				write("syscall");
				write("sw $v0, {}", instr.target);
				releaseRegisters({ v0 });
				break;
			case Instr::SCAN_CHAR:
				getRegisters({ v0 });
				write("li $v0, 12");
				write("syscall");
				write("sw $v0, {}($fp)", instr.target);
				releaseRegisters({ v0 });
				break;

			case Instr::SAVE_STA_ARR:
				write("sw {}, ({})", instr.target, instr.source_a);
				break;
			case Instr::LOAD_STA_ARR:
				write("lw {}, ({})", instr.target, instr.source_a);
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
				pushRegPool();
				break;
			case Instr::POP_REGPOOL:
				popRegPool();
				break;
			case Instr::IR_SHOW: break;
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
