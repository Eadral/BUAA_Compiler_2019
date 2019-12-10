#pragma once

#include "../meow/core.hpp"
#include "ir.hpp"
#include "interpreter.hpp"
#include "flowgraph.hpp"
#include "define_arrival.hpp"
#include "define_use_chain.hpp"

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

			inlineFuncs();
			
			// constantProgpagation();
			// removeZeroLoad();
			// regAssign();
			
		}

		void unitTestDefineUseChain() {
			FlowGraph flow_graph;
			flow_graph.addEdge("B1", "B2");
			flow_graph.addEdge("B2", "B3");
			flow_graph.addEdge("B2", "B4");
			flow_graph.addEdge("B3", "B2");
			flow_graph.addEdge("B4", "B5");
			flow_graph.addEdge("B5", "B6");
			flow_graph.addEdge("B5", "exit");
			flow_graph.addEdge("B6", "B5");

			DefineUseChain define_use_chain;
			define_use_chain.addDef(1, "B1", 1, "a");
			define_use_chain.addDef(1, "B1", 2, "i");
			// define_use_chain.addDef(1, "B1", 3, "i");
			
			define_use_chain.addUse(2, "B2", 1, "i");
			
			define_use_chain.addDef(3, "B3", 1, "a");
			define_use_chain.addUse(3, "B3", 1, "a");
			define_use_chain.addUse(3, "B3", 1, "i");

			define_use_chain.addDef(3, "B3", 2, "i");
			define_use_chain.addUse(3, "B3", 2, "i");

			define_use_chain.addDef(4, "B4", 1, "b");
			define_use_chain.addUse(4, "B4", 1, "a");
			define_use_chain.addDef(4, "B4", 2, "i");
			
			define_use_chain.addUse(5, "B5", 1, "i");
			
			define_use_chain.addDef(6, "B6", 1, "b");
			define_use_chain.addUse(6, "B6", 1, "b");
			define_use_chain.addUse(6, "B6", 1, "i");

			define_use_chain.addDef(6, "B6", 2, "i");
			define_use_chain.addUse(6, "B6", 2, "i");

			
			define_use_chain.generate(flow_graph);
			define_use_chain.printChain();
		}


		void unitTestDefineArrival() {
			FlowGraph flow_graph;
			flow_graph.addEdge("B1", "B2");
			flow_graph.addEdge("B2", "B3");
			flow_graph.addEdge("B2", "exit");
			flow_graph.addEdge("B3", "B4");
			flow_graph.addEdge("B3", "B5");
			flow_graph.addEdge("B4", "B5");
			flow_graph.addEdge("B5", "B2");

			DefineArrival define_arrival;
			define_arrival.addGen("B1", vector<string>{"d1", "d2", "d3"});
			define_arrival.addKill("B1", vector<string>{"d5", "d6", "d7", "d8"});

			define_arrival.addGen("B3", vector<string>{"d4", "d5"});
			define_arrival.addKill("B3", vector<string>{"d1", "d6"});

			define_arrival.addGen("B4", vector<string>{"d6"});
			define_arrival.addKill("B4", vector<string>{"d1", "d5"});

			define_arrival.addGen("B5", vector<string>{"d7", "d8"});
			define_arrival.addKill("B5", vector<string>{"d2", "d3"});

			define_arrival.generate(flow_graph);
			define_arrival.iterate(flow_graph);
			// define_arrival.printInOut();
			// TODO: p350 artificial check
		}


		void unitTests() {
			unitTestDefineArrival();

		}
		
#pragma region Utils

#define EndFor }
		
		Instr getJumpInstr(Block& block) {
			Instr nop = Instr( Instr::NOP );
			if (block.instrs.empty()) {
				return nop;
			}
			Instr last = block.instrs.back();
			if (last.isJump()) {
				return last;
			} else {
				return nop;
			}
		}
		
		void insertBefore(vector<Instr>& instrs, int& i, Instr instr) {
			instrs.insert(instrs.begin() + i, instr);
			i++;
		}

		void insertAfter(vector<Instr>& instrs, int& i, Instr instr) {
			instrs.insert(instrs.begin() + i + 1, instr);
		}

		void deleteThis(vector<Instr>& instrs, int& i, Instr instr) {
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
		Func& getFuncByName(string name) {
			for (int i = 0; i < ir.funcs.size(); i++) {
				if (ir.funcs[i].func_name == name) {
					return ir.funcs[i];
				}
			}
			panic("aho no this func");
			// return Func("");
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
					
				EndFor
			EndFor
		}

		void constProgpaBlock(Block& block) {
			Interpreter interpreter;

			ForInstrs(i, block.instrs, instr)
				interpreter.eval(instr);
			EndFor
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
			EndFor
			
		}
		std::map<string, int> load_cnt;


		void removeZeroLoadInBlock(Block& block) {
	
			for (int i = block.instrs.size()-1; i >= 0; i--) {
				auto& instr = block.instrs.at(i);

				if (!blockHasJump(block)) {
					// TODO: change to down jump
					// TODO: remove this
					auto save_name = instr.getStoreName();
					if (!save_name.empty()
						&& !starts_with(save_name, string("$"))
						&& !starts_with(save_name, string("__G"))
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

#pragma region FunctionInline

		void inlineFuncs() {
			ForFuncs(i, func)
				if (!hasFuncCall(func))
					inlineFunc(func);
			EndFor
		}
		
		void inlineFunc(Func &func) {
			auto paras = getParas(func);
			removeFuncPara(func, paras);
			// is, func_name, block_index, line_number;
			while (true) {
				bool found;
				string func_name;
				int block_index, line_number;
				tie(found, func_name, block_index, line_number) = findFuncCall(func.func_name);
				if (!found)
					break;

				Func& fromfunc = getFuncByName(func_name);
				fromfunc.blocks->at(block_index).instrs[line_number - 1] = Instr(Instr::NOP); // remove $fp = $sp + x;

				
				
			}
			
			
			// TODO: removeFunc(func);
		}

		void removeBeforeFuncCall(Func& func, int block_index, int line_number) {
			
		}

		void removeAfterFuncCall(Func& func, int block_index, int line_number) {
			
		}

		void replaceFuncCall() {
			
		}

		void removeFuncPara(Func &func, vector<string> paras) {
			// int index = 0;
			ForBlocks(j, func.blocks, block)
				if (block.label == FORMAT("return_{}", func.func_name)) {
					block.instrs.clear();
					continue;
				}
				ForInstrs(k, block.instrs, instr)
					if (instr.type == Instr::PARA)
						instr = Instr(Instr::NOP);
					if (instr.type == Instr::LOAD_STA) {
						int index = getFoundIndex(paras, instr.showas);
						if (index == -1)
							continue;
						instr = Instr(Instr::MOVE, instr.target, getInlineTempName(func.func_name, index, paras[index]));
					}
					// if (instr.type == Instr::JUMP && instr.target == FORMAT("return_{}", func.func_name))
					// 	instr = Instr(Instr::NOP);
				EndFor
			EndFor
		}

		int getFoundIndex(vector<string> paras, string x) {
			for (int i = 0; i < paras.size(); i++) {
				if (paras[i] == x)
					return i;
			}
			return -1;
		}

		// is, func_name, block_index, line_number
		tuple<bool, string, int, int> findFuncCall(string func_name) {
			ForFuncs(i, func)
			
				ForBlocks(j, func.blocks, block)
						ForInstrs(k, block.instrs, instr)
							if (instr.type == Instr::CALL && instr.target == func_name) {
								// while (!(instrs[k].type == Instr::PUSH_REG && instr.target == "$fp")) {
								// 	k--;
								// }
								return make_tuple(true, func.func_name, j, k);
							}
						EndFor
					EndFor
				EndFor
			return make_tuple(false, "", 0, 0);
		}

		vector<string> getParas(Func &func) {
			vector<string> idents;
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					if (instr.type == Instr::PARA) {
						idents.push_back(instr.source_a);
					}
				EndFor
			EndFor
			return idents;
		}

		string getInlineTempName(string func_name, int index, string ident) {
			return FORMAT("__T{}_{}_{}", func_name, index, ident);
		}
		
#pragma endregion 

		// FlowGraph constructFlowGraph() {
		//
		// 	ForFuncs(i, func)
		// 		
		// 		// if (func.func_name == "main") {
		// 		// 	flow_graph.addEdge(flow_graph.entry, func.blocks->at(0).label);
		// 		// 	flow_graph.addEdge(func.blocks->back().label, flow_graph.exit);
		// 		// }
		// 	EndFor
		// 	return flow_graph;
		// }

		FlowGraph constructFlowGraphFunc(Func &func) {
			FlowGraph flow_graph;

			flow_graph.addEntry(func.func_name);
			ForBlocks(j, func.blocks, block)
				if (j + 1 < blocks->size()) {
					Instr jump_instr = getJumpInstr(block);
					if (jump_instr.type != Instr::NOP && jump_instr.type != Instr::CALL) {
						flow_graph.addEdge(block.label, jump_instr.getJumpTarget());
					}
					if (jump_instr.type != Instr::JUMP) {
						flow_graph.addEdge(blocks->at(j).label, blocks->at(j + 1).label);
					}
				}
			EndFor

			return flow_graph;
		}

#pragma region RegAssign
		
		void regAssign() {
			ForFuncs(i, func)
				regAssignFunc(func);
			EndFor
		}

		bool needAssign(string reg) {
			return !reg.empty() && !starts_with(reg, string("$"));
				
		}

		bool hasDefAfter(Block &block, int block_line_number, string use) {
			for (int i = block_line_number+1; i < block.instrs.size(); i++) {
				if (block.instrs[i].getStoreName() == use) {
					return true;
				}
			}
			return false;
		}
		
		void regAssignFunc(Func& func) {
			FlowGraph flow_graph = constructFlowGraphFunc(func);
			DefineUseChain define_use_chain;
			
			ForBlocks(block_index, func.blocks, block)
				
				ForInstrs(line_number_in_block, block.instrs, instr)
					if (instr.type == Instr::IR_SHOW)
						continue;
					auto loads = instr.getLoadName();
					for (auto load : loads) {
						if (needAssign(load))
							define_use_chain.addUse(block_index, block.label, line_number_in_block, load);
					}
					auto use = instr.getStoreName();
					if (needAssign(use) && !hasDefAfter(block, line_number_in_block, use))
						define_use_chain.addDef(block_index, block.label, line_number_in_block, use);
					instr.block_line_number = line_number_in_block;

				EndFor
			EndFor

			define_use_chain.generate(flow_graph);
			ir.func_to_ident_to_range[func.func_name] = define_use_chain.getRanges();
			
			// define_use_chain.printChain();
			
		}

#pragma  endregion 
		
	};
	
}
