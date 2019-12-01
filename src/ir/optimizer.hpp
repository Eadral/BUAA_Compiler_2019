#pragma once

#include "../meow/core.hpp"
#include "ir.hpp"
#include "interpreter.hpp"

namespace buaac {

	class Optimizer {
	public:
		IR ir;

		Optimizer(IR ir) : ir(ir) {}


		IR getIR() {
			return ir;
		}

		// OPTIMIZE
		void optimize() {
			removeRa(getFuncByName("main"));
			removeUselessRa();
			constantProgpagation();
		}

#pragma region Utils

#define ForFuncs(i, func)	 for (int i = 0; i < ir.funcs.size(); i++) { auto& funcs = ir.funcs; auto& func = ir.funcs.at(i);

#define ForBlocks(j, _blocks, block) for (int j = 0; j < _blocks->size(); j++) { auto& blocks = _blocks; auto& block = _blocks->at(j);

#define ForInstrs(k, _instrs, instr) for (int k = 0; k < _instrs.size(); k++) { auto& instrs = _instrs; auto& instr = _instrs[k]; 

		
		void insertBefore(std::vector<Instr>& instrs, int& i, Instr instr) {
			instrs.insert(instrs.begin() + i, instr);
			i++;
		}

		void insertAfter(std::vector<Instr>& instrs, int& i, Instr instr) {
			instrs.insert(instrs.begin() + i + 1, instr);
		}

		void deleteThis(std::vector<Instr>& instrs, int& i, Instr instr) {
			instrs.erase(instrs.begin() + i);
			
		}

		// methods
		void removeUselessRa() {
			for (int i = 0; i < ir.funcs.size(); i++) {
				if (!hasFuncCall(ir.funcs[i])) {
					removeRa(ir.funcs[i]);
				}
			}
		}
		
		// helper
		Func& getFuncByName(std::string name) {
			for (int i = 0; i < ir.funcs.size(); i++) {
				if (ir.funcs[i].func_name == name) {
					return ir.funcs[i];
				}
			}
			panic("can not find this func");
		}

		// checker
		bool hasFuncCall(Func& func) {
			for (int i = 0; i < func.blocks->size(); i++) {
				auto& block = func.blocks->at(i);
				auto& instrs = block.instrs;
				for (int j = 0; j < instrs.size(); j++) {
					auto& instr = instrs[j];
					if (instr.type == Instr::CALL)
						return true;
						
				}
			}
			return false;
		}

		// modifier
		void removeRa(Func& func) {
			for (int i = 0; i < func.blocks->size(); i++) {
				auto& block = func.blocks->at(i);
				auto& instrs = block.instrs;
				for (int j = 0; j < instrs.size(); j++) {
					auto& instr = instrs[j];
					if ( (instr.type == Instr::PUSH_REG || instr.type == Instr::POP_REG )
						&& instr.target == "$ra") {
						deleteThis(instrs, j, instr);
					}
				}
			}
		}
#pragma endregion 

		
#pragma region ConstantProgpagation


		void constantProgpagation() {
			ForFuncs(i, func)
				ForBlocks(j, func.blocks, block)
					constProgpaBlock(block);
				}
			}
		}

		void constProgpaBlock(Block& block) {
			Interpreter interpreter;

			ForInstrs(i, block.instrs, instr)
				interpreter.eval(instr);
			}

	
	
		}
		
#pragma endregion 
		
	};
	
}
