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
			// constantProgpagation();
			// removeZeroLoad();
			// FlowGraph flow_graph = constructFlowGraph();
			// flow_graph.printGraph();
			regAssign();
			
			// unitTestDefineUseChain();
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

		void regAssign() {
			ForFuncs(i, func)
				regAssignFunc(func);
			EndFor
		}

		bool needAssign(string reg) {
			return !reg.empty() && !starts_with(reg, string("$"));
				
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
					if (needAssign(use))
						define_use_chain.addDef(block_index, block.label, line_number_in_block, use);

				EndFor
			EndFor

			define_use_chain.generate(flow_graph);
			ir.func_to_ident_to_range[func.func_name] = define_use_chain.getRanges();
			define_use_chain.printChain();
			
		}

	};
	
}
