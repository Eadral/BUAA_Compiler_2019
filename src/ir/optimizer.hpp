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
			removeZeroLoad();
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

		bool blockHasJump(Block& block) {
			if (block.instrs.empty())
				return false;
			return block.instrs.at(block.instrs.size() - 1).isJump();
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

		void removeZeroLoad() {
			load_cnt.clear();
			ForFuncs(i, func)
				for (int j = func.blocks->size()-1; j >= 0; j--) {
					auto& blocks = func.blocks;
					auto& block = func.blocks->at(j);
					
						// TODO: remove this jump
						removeZeroLoadInBlock(block);

					

				}
			}
			
		}
		std::map<std::string, int> load_cnt;


		void removeZeroLoadInBlock(Block& block) {
	
			for (int i = block.instrs.size()-1; i >= 0; i--) {
				auto& instr = block.instrs.at(i);

				if (!blockHasJump(block)) {
					// TODO: change to down jump
					// TODO: remove this
					auto save_name = instr.getStoreName();
					if (!save_name.empty()
						&& !starts_with(save_name, std::string("$"))
						&& !starts_with(save_name, std::string("__G"))
						&& !instr.doNotDelDead()
						&& load_cnt.find(save_name) == load_cnt.end()) {
						instr.type = Instr::NOP;
						continue;
					}
				}
				
				auto load_names = instr.getLoadName();
				if (instr.isMemorySave()) {
					load_names.push_back(instr.target);
				}
				for (int j = 0; j < load_names.size(); j++) {
					auto& name = load_names[j];
					if (load_cnt.find(name) == load_cnt.end()) {
						load_cnt[name] = 1;
					}
				}
				
			}

	
		}

#pragma endregion


		
		
	};
	
}
