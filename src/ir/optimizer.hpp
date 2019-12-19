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

			// unitTestDefineUseChain();

			removeFuncsVars();
			inlineFuncs();
			for (int i = 0; i < 3; i++) {
				loopUnroll();
				blockMerge();
				constantProgpagation();
				// copyPropagation();
				// ALUPropagation();
				removeZeroLoad();
				removeZeroLoadGlobal();
			}
			

			
			// if (maxLoopCnt() >= 100) {
			// 	
			// 	loopUnroll();
			// 	blockMerge();
			//
			//
			// 	constantProgpagation();
			//
			//
			//
			// 	for (int k = 0; k < 50; k++) {
			// 		if (k % 20 == 0)
			// 			removeNop();
			// 		loopUnroll();
			// 		bool flag;
			// 		do {
			// 			flag = false;
			// 			constantProgpagation();
			// 			// if (oneForAll())
			// 			// 	flag = true;
			// 			// if (blockMergeJump())
			// 				// flag = true;
			// 			// if (blockMergeSimple())
			// 				// flag = true;
			// 			// constantProgpagation();
			// 			if (blockMergeNoEntry())
			// 				flag = true;
			// 			if (blockMergeJump())
			// 				flag = true;
			// 			if (blockMergeSimple())
			// 				flag = true;
			// 		} while (flag);
			// 		
			// 		
			// 	}
			// 	
			// 	removeZeroLoad();
			// 	removeZeroLoadGlobal();
			// 	removeDeadSave();
			// } else {
			// 	constantProgpagation();
			// 	copyPropagation();
			// 	ALUPropagation();
			// 	removeZeroLoad();
			// 	removeZeroLoadGlobal();
			// 	removeDeadSave();
			// }

			No_Need_Unroll:
			
			regAssign();
			
		}

		void removeNop() {
			ForFuncs(i, func)
				ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					if (instr.type == Instr::NOP)
						instrs.erase(instrs.begin() + k--);
					
					EndFor
				EndFor
			EndFor
		}

		void removeDeadSave() {
			ForFuncs(i, func)
				removeDeadSaveFunc(func);
			EndFor
		}

		void removeDeadSaveFunc(Func &func) {
			ForBlocks(j, func.blocks, block)
				map<string, bool> is_dead_save;
				for (int i = block.instrs.size()-1; i >= 0; i--) {
					auto& instr = block.instrs.at(i);

					auto save = instr.getStoreName();
					if (!save.empty()
						&& (instr.target == "$v0" || !starts_with(save, string("$")))
						&& !starts_with(save, string("__g"))
						&& !instr.doNotDelDead()
						&& is_dead_save[save] == true
						
						) {
						instr = Instr(Instr::NOP);
						continue;
					}
					is_dead_save[save] = true;

					auto loads = instr.getLoadName();
					for (auto &load : loads) {
						is_dead_save[load] = false;
					}
					
				}
			EndFor
		}


#pragma region UnitTest
		
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

#pragma endregion 
		
		
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

		FlowGraph constructFlowGraphFunc(Func& func) {
			FlowGraph flow_graph;

			// flow_graph.addEntry(func.func_name);
			ForBlocks(j, func.blocks, block)
				flow_graph.addId(block.label, j);
				if (j + 1 < blocks->size()) {
					Instr &jump_instr = block.instrs.back();
					if (jump_instr.isJump() && jump_instr.type != Instr::CALL) {
						flow_graph.addEdge(block.label, jump_instr.getJumpTarget());
					}
					if (jump_instr.type != Instr::JUMP) {
						flow_graph.addEdge(blocks->at(j).label, blocks->at(j + 1).label);
					}
				}
			EndFor

				return flow_graph;
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
					
					for (int i = block.instrs.size() - 1; i >= 0; i--) {
						auto& instr = block.instrs.at(i);

						if (!blockHasJump(block)) {
							// TODO: change to down jump
							// TODO: remove this
							auto save_name = instr.getStoreName();
							if (!save_name.empty()
								&& (!starts_with(save_name, string("$")))
								&& !starts_with(save_name, string("__G"))
								&& !instr.doNotDelDead()
								&& load_cnt.find(save_name) == load_cnt.end()) {
								instr.type = Instr::NOP;
								continue;
							}
						}

						auto load_names = instr.getLoadName();
						// if (instr.isMemorySave()) {
						// 	load_names.push_back(instr.target);
						// }
						for (int j = 0; j < load_names.size(); j++) {
							auto& name = load_names[j];
							if (load_cnt.find(name) == load_cnt.end()) {
								load_cnt[name] = 1;
							}
						}

					}

					

				}
			EndFor
			
		}
		std::map<string, int> load_cnt;

		void removeZeroLoadGlobal() {
			ForFuncs(i, func)
				removeZeroLoadFunc(func);
			EndFor
		}

		void removeZeroLoadFunc(Func& func) {
			map<string, int> load_cnt;

			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					auto loads = instr.getLoadName();
					for (auto load : loads) {
						load_cnt[load]++;
					}
				EndFor
			EndFor

			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					auto save_name = instr.getStoreName();
					if (!save_name.empty()
						&& !starts_with(save_name, string("$"))
						&& !starts_with(save_name, string("__g"))
						&& !instr.doNotDelDead()
						&& load_cnt.find(save_name) == load_cnt.end()) {
						instr.type = Instr::NOP;
						continue;
					}
				EndFor
			EndFor
			
		}
		
		//
		// void removeZeroLoadInBlock(Block& block) {
		//
		// 	
		//
		//
		// }

#pragma endregion

		
#pragma region FunctionInline

		
		
		void inlineFuncs() {
			bool flag = false;
			do {
				removeUselessRa();
				flag = false;
				ForFuncs(i, func)
					if (!hasLocalArr(func) && !hasFuncCall(func) &&!hasLoop(func) && func.func_name != "main") {
						flag = true;
						inlineFunc(func);
						// remove
						ir.funcs.erase(ir.funcs.begin() + i);
						i--;
					}
				EndFor
			} while (flag);
			
		}

		void inlineFuncsNoVar() {
			bool flag = false;
			do {
				removeUselessRa();
				flag = false;
				ForFuncs(i, func)
					if (!hasLocalArr(func) && !hasFuncCall(func) && !hasLocalVar(func) && func.func_name != "main") {
						flag = true;
						inlineFunc(func);
						// remove
						ir.funcs.erase(ir.funcs.begin() + i);
						i--;
					}
				EndFor
			} while (flag);

		}

		bool hasLoop(Func& func) {
			ForBlocks(j, func.blocks, block)
				if (starts_with(block.label, string("for")) 
					|| starts_with(block.label, string("while"))
					|| starts_with(block.label, string("do"))
					)
					return true;
			EndFor
			return false;
		}

		bool hasLocalArr(Func& func) {
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
				if (instr.type == Instr::VAR && instr.source_a != "0")
					return true;
				EndFor
			EndFor
			return false;
		}

		bool hasLocalVar(Func& func) {
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
				if (instr.type == Instr::VAR)
					return true;
				EndFor
			EndFor
			return false;
		}
		
		void inlineFunc(Func &func) {
			auto paras = getParas(func);
			removeFuncPara(func, paras);
			// is, func_name, block_index, line_number;
			int cnt = 0;
			while (true)
			{
				cnt++;
				bool found;
				string fromfunc_name;
				int block_index, line_number;
				tie(found, fromfunc_name, block_index, line_number) = findFuncCall(func.func_name);
				if (!found)
					break;

				Func& fromfunc = getFuncByName(fromfunc_name);

				
				removeBeforeFuncCall(func.func_name, fromfunc, block_index, line_number, paras, cnt);
				removeAfterFuncCall(func.func_name, fromfunc, block_index, line_number);
				removeFuncCall(func.func_name, fromfunc, block_index, line_number);

				// replace Function
				vector<string> labels;
				for (int i = 0; i < func.blocks->size(); i++) {
					labels.push_back(func.blocks->at(i).label);
				}
				
				for (int i = func.blocks->size() - 1; i >= 0; i--) {
					auto new_block = func.blocks->at(i);
					new_block.label = FORMAT("inline_{}_{}", cnt, new_block.label);
					for (int j = 0; j < new_block.instrs.size(); j++) {
						auto& instr = new_block.instrs[j];
						if (instr.isJump() && getFoundIndex( labels, instr.getJumpTarget()) != -1) {
							instr.changeJumpTarget(FORMAT("inline_{}_{}", cnt, instr.getJumpTarget()));
						}
						// if (instr.type == Instr::MOVE && starts_with(instr.source_a, FORMAT("__T{}", func.func_name))) {
						// 	instr.source_a = FORMAT("{}_{}", instr.source_a, cnt);
						// }
						// if (instr.type == Instr::MOVE && starts_with(instr.target, FORMAT("__T{}", func.func_name))) {
						// 	instr.target = FORMAT("{}_{}", instr.target, cnt);
						// }
						if (starts_with(instr.target, string("__T"))) {
							instr.target = FORMAT("{}_{}", instr.target, cnt);
						}
						if (starts_with(instr.source_a, string("__T"))) {
							instr.source_a = FORMAT("{}_{}", instr.source_a, cnt);
						}
						if (starts_with(instr.source_b, string("__T"))) {
							instr.source_b = FORMAT("{}_{}", instr.source_b, cnt);
						}
					}
					fromfunc.blocks->insert(fromfunc.blocks->begin() + block_index + 1, new_block );
				}
				
			}
			
			// TODO: removeFunc(func);
		}

		

		void removeBeforeFuncCall(string inline_func_name, Func& fromfunc, int block_index, int line_number, vector<string> paras, int cnt) {
			auto i = InstrIterator(fromfunc, block_index, line_number);
			
			int index = paras.size() - 1;
			while (index >= 0) {
				i.previous();
				if (i.getInstr().type == Instr::PUSH_REG && i.getInstr().source_a == inline_func_name && i.getInstr().source_b == i2a(index)) {
					i.getInstr() = Instr(Instr::MOVE, getInlineTempNameWithCnt(inline_func_name, index, paras[index], cnt), i.getInstr().target);
					index--;
				}
			}

			while (!(i.getInstr().type == Instr::PUSH_REG && i.getInstr().target == "$fp" && i.getInstr().source_a == inline_func_name)) {
				i.previous();
			}
			i.getInstr() = Instr(Instr::NOP);

			i.next();
			i.getInstr() = Instr(Instr::NOP);	// remove push regpool
		}

		void removeAfterFuncCall(string inline_func_name, Func& fromfunc, int block_index, int line_number) {
			auto i = InstrIterator(fromfunc, block_index, line_number);

			i.next();
			i.getInstr() = Instr(Instr::NOP);

			i.next();
			i.getInstr() = Instr(Instr::NOP);
			
			i.next();
			i.getInstr() = Instr(Instr::NOP);
			
			// while (!(i.getInstr().type == Instr::POP && i.getInstr().source_a == inline_func_name)) {
			// 	i.next();
			// }
			// i.getInstr() = Instr(Instr::NOP);
			//
			// while (!(i.getInstr().type == Instr::POP_REG && i.getInstr().target == "$fp" && i.getInstr().source_a == inline_func_name)) {
			// 	i.next();
			// }
			// i.getInstr() = Instr(Instr::NOP);
			
			
		}

		void removeFuncCall(string inline_func_name, Func& fromfunc, int block_index, int line_number) {
			auto i = InstrIterator(fromfunc, block_index, line_number);
			i.getInstr() = Instr(Instr::NOP); // fp
			i.previous();
			i.getInstr() = Instr(Instr::NOP); // call
			// i.next();
			// i.next();
			// i.getInstr() = Instr(Instr::NOP);
		}

		vector<string> getFuncVars(Func &func) {
			vector<string> idents;
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					if (instr.type == Instr::VAR && instr.source_a == "0") {
						idents.push_back(instr.target);
					}
				EndFor
			EndFor
			return idents;
		}

		void removeFuncVars(Func& func, vector<string> idents) {
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					if (instr.type == Instr::VAR && getFoundIndex(idents, instr.target) != -1) {
						instr = Instr(Instr::NOP);
					}
					if (instr.type == Instr::LOAD_STA) {
						int index = getFoundIndex(idents, instr.showas);
						if (index == -1)
							continue;
						instr = Instr(Instr::MOVE, instr.target, getVarTempName(idents[index]));
					}
					if (instr.type == Instr::SAVE_STA) {
						int index = getFoundIndex(idents, instr.showas);
						if (index == -1)
							continue;
						instr = Instr(Instr::MOVE, getVarTempName(idents[index]), instr.target);
					}
					if (instr.type == Instr::SCAN_INT || instr.type == Instr::SCAN_CHAR) {
						int index = getFoundIndex(idents, instr.showas);
						if (index == -1)
							continue;
						instr.target = getVarTempName(idents[index]);
						instr.showas = getVarTempName(idents[index]);
					}
				EndFor
			EndFor
		}

		void removeFuncsVars() {
			ForFuncs(i, func)
				auto idents = getFuncVars(func);
				removeFuncVars(func, idents);
			EndFor
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
					if (instr.type == Instr::PUSH)
						instr = Instr(Instr::NOP);
					if (instr.type == Instr::LOAD_STA) {
						int index = getFoundIndex(paras, instr.showas);
						if (index == -1)
							continue;
						instr = Instr(Instr::MOVE, instr.target, getInlineTempName(func.func_name, index, paras[index]));
					}
					if (instr.type == Instr::SAVE_STA) {
						int index = getFoundIndex(paras, instr.showas);
						if (index == -1)
							continue;
						instr = Instr(Instr::MOVE, getInlineTempName(func.func_name, index, paras[index]), instr.target);
					}
					// if (instr.type == Instr::JUMP && instr.target == FORMAT("return_{}", func.func_name))
					// 	instr = Instr(Instr::NOP);
				EndFor
			EndFor
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
			return FORMAT("__T0{}_{}_{}", func_name, index, ident);
		}

		string getVarTempName(string ident) {
			return FORMAT("__T{}", ident);
		}

		string getInlineTempNameWithCnt(string func_name, int index, string ident, int cnt) {
			return FORMAT("{}_{}", getInlineTempName(func_name, index, ident), cnt);
		}
		
#pragma endregion 

		
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
					// TODO: bug here: deref
					if (needAssign(use) )
						define_use_chain.addDef(block_index, block.label, line_number_in_block, use);
					instr.block_line_number = line_number_in_block;

				EndFor
			EndFor

			define_use_chain.generate(flow_graph);
			ir.func_to_ident_to_range[func.func_name] = define_use_chain.getRanges();

#ifdef SHOWALLOC
			define_use_chain.printChain();
#endif
			
		}

#pragma  endregion 

#pragma region ALUPropagation

		void ALUPropagation() {
			ForFuncs(i, func)
				ALUPropagationFunc(func);
			EndFor
		}

		void ALUPropagationFunc(Func &func) {
			ForBlocks(j, func.blocks, block)
				map<string, string> copy;

				ForInstrs(k, block.instrs, instr)

					if (
						instr.isALU()
						&& instr.type != Instr::DIV
						// && !starts_with(instr.source_a, string("$"))
						)
					{

						// copy[instr.target] = instr.source_a;
						int end = findNextSave(block, k, instr.source_a) + 1;
						end = std::min(end, findNextSave(block, k, instr.source_b) + 1);
						end = std::min(end, (int)block.instrs.size());

						replaceRangeMoveWithInstr(block, k + 1, end, instr.target, instr);

					}

				EndFor
			EndFor
		}

		void replaceRangeMoveWithInstr(Block& block, int start, int end, string from, Instr to) {
			for (int i = start; i < end; i++) {
				auto& instr = block.instrs.at(i);
				if (instr.type == Instr::MOVE && instr.source_a == from) {
					to.target = instr.target;
					instr = to;
				}
				
			}
		}

		// int findNextMoveLoad(Block& block, int start, string load_name) {
		// 	for (int i = start; i < block.instrs.size(); i++) {
		// 		auto& instr = block.instrs.at(i);
		// 		if (instr.type ==Instr::MOVE && !instr.getLoadName().empty() && instr.getLoadName()[0] == load_name) {
		// 			return i;
		// 		}
		// 	}
		// 	return INT32_MAX;
		// }
		
#pragma endregion 
		
#pragma region CopyPropagation
		
		void copyPropagation() {
			ForFuncs(i, func)
				copyPropagationFuncConservative(func);
				copyPropagationFuncBeforeNextDef(func);
			EndFor
		}

		void copyPropagationFuncConservative(Func &func) {
			
			// FlowGraph flow_graph = constructFlowGraphFunc(func);

			ForBlocks(j, func.blocks, block)
				map<string, string> copy;

				ForInstrs(k, block.instrs, instr)

					if (instr.targetIsLoad() && found(copy, instr.target)) {
						instr.target = copy[instr.target];
					}
					if (instr.sourceAIsLoad() && found(copy, instr.source_a)) {
						instr.source_a = copy[instr.source_a];
					}
					if (instr.sourceBIsLoad() && found(copy, instr.source_b)) {
						instr.source_b = copy[instr.source_b];
					}
			
					if (instr.type == Instr::MOVE 
						&& starts_with(instr.source_a, string("__T")) 
						&& !starts_with(instr.source_a, string("$"))
						&& noSaveAfter(instr.source_a, block, k)
						)

					{
						copy[instr.target] = instr.source_a;
					}
					
				EndFor
			EndFor
		}

		void copyPropagationFuncBeforeNextDef(Func &func) {
			ForBlocks(j, func.blocks, block)
				map<string, string> copy;

					ForInstrs(k, block.instrs, instr)

						if (instr.type == Instr::MOVE
							&& starts_with(instr.source_a, string("__T"))
							&& !starts_with(instr.source_a, string("$"))
							)
						{
							
							// copy[instr.target] = instr.source_a;
							int end = findNextSave(block, k, instr.source_a);
							end = std::min(end, (int)block.instrs.size());

							replaceRangeLoad(block, k+1, end, instr.target, instr.source_a);
							
						}

					EndFor
				EndFor
		}

		void replaceRangeLoad(Block &block, int start, int end, string from, string to) {
			for (int i = start; i < end; i++) {
				auto& instr = block.instrs.at(i);
				if (instr.targetIsLoad() && instr.target == from) {
					instr.target = to;
				}
				if (instr.sourceAIsLoad() && instr.source_a == from) {
					instr.source_a = to;
				}
				if (instr.sourceBIsLoad() && instr.source_b == from) {
					instr.source_b = to;
				}
			}
		}

		int findNextSave(Block& block, int start, string save_name) {
			if (save_name.empty() || !starts_with(save_name, string("__T"))) {
				return INT32_MAX - 1;
			}
			for (int i = start; i < block.instrs.size(); i++) {
				auto& instr = block.instrs.at(i);
				if (instr.getStoreName() == save_name) {
					return i;
				}
			}
			return INT32_MAX - 1;
		}


		bool noSaveAfter(string save_name, Block &block, int line_number) {
			for (int i = line_number; i < block.instrs.size(); i++) {
				auto& instr = block.instrs.at(i);
				if (instr.getStoreName() == save_name) {
					return false;
				}
			}
			return true;
		}

#pragma endregion 


#pragma region BlockMerge

		void blockMerge() {
			blockMergeEmpty();
			blockMergeJump();
			blockMergeSimple();
			blockMergeNoEntry();
			clearUselessJump();
			blockMergeSimple();
		}

		void clearUselessJump() {
			ForFuncs(i, func)
				clearUselessJumpFunc(func);
			EndFor
		}

		void clearUselessJumpFunc(Func &func) {
			ForBlocks(j, func.blocks, block)
				if (block.instrs.size() == 0 || j >= blocks->size() - 2)
					continue;
				auto& back_instr = block.instrs.back();
				if (!(back_instr.type == Instr::JUMP || back_instr.type == Instr::RETURN_END)) {
					continue;
				}
				auto next_label = blocks->at(j + 1).label;
				if (back_instr.getJumpTarget() == next_label)
					back_instr = Instr(Instr::NOP);
			EndFor
		}
		
		void blockMergeEmpty() {
			ForFuncs(i, func)
				blockMergeEmptyFunc(func);
			EndFor
		}

		void blockMergeEmptyFunc(Func &func) {
			ForBlocks(j, func.blocks, block)
				if (block.instrs.size() == 0 && j < blocks->size()-1) {
					changeJumpInFunc(func, blocks->at(j).label, blocks->at(j + 1).label);
					blocks->erase(blocks->begin() + j);
					j--;
				}
			EndFor
		}

		void changeJumpInFunc(Func &func, string from, string to) {
			ForBlocks(j, func.blocks, block)
				ForInstrs(k, block.instrs, instr)
					if (instr.isJump() && instr.getJumpTarget() == from) {
						instr.changeJumpTarget(to);
					}
				EndFor
			EndFor
		}

		void changeJumpInBlocks(vector<Block> &blocks, string from, string to) {
			for (auto &block : blocks) {
				ForInstrs(k, block.instrs, instr)
					if (instr.isJump() && instr.getJumpTarget() == from) {
						instr.changeJumpTarget(to);
					}
				EndFor
			}
		}
		
		bool blockMergeJump() {
			bool ans = false;
			ForFuncs(i, func)
				if (blockMergeJumpFunc(func)) {
					ans = true;
				}
			EndFor
			return ans;
		}

		bool blockMergeJumpFunc(Func &func) {
			bool ans = false;
			bool flag;
			do {
				flag = false;
				FlowGraph flow_graph = constructFlowGraphFunc(func);

				ForBlocks(j, func.blocks, block)
					if (block.instrs.empty() || j >= blocks->size() - 2)
						break;
					auto& back_instr = block.instrs.back();
					if (!(back_instr.type == Instr::JUMP)) {
						continue;
					}
					auto next_label = blocks->at(j + 1).label;
					if (back_instr.getJumpTarget() == next_label && !hasMultiEntry(flow_graph, next_label)) {
						back_instr = Instr(Instr::NOP);
						ForInstrs(k, blocks->at(j + 1).instrs, instr)
							blocks->at(j).instrs.push_back(instr);
						EndFor
						blocks->erase(blocks->begin() + j + 1);
						flag = true;
						ans = true;
					}
				EndFor
				
			} while (flag);
				return ans;
			
		}

		bool hasNoEntry(FlowGraph &flow_graph, string label) {
			return flow_graph.getPreds(label).size() == 0;
		}

		bool hasOnlySelfLoop(FlowGraph& flow_graph, string label) {
			auto preds = flow_graph.getPreds(label);
			if (preds.size() == 1 && preds[0] == label) {
				return true;
			}
			return false;
		}
		
		bool hasMultiEntry(FlowGraph &flow_graph, string label) {
			return flow_graph.getPreds(label).size() > 1;
		}

		bool blockMergeSimple() {
			bool ans = false;
			ForFuncs(i, func)
				if (blockMergeSimpleFunc(func)) {
					ans = true;
				}
			EndFor
			return ans;
		}

		bool blockMergeSimpleFunc(Func &func) {
			bool ans = false;
			bool flag;
			do {
				flag = false;
				FlowGraph flow_graph = constructFlowGraphFunc(func);

				ForBlocks(j, func.blocks, block)
					if (block.instrs.size() == 0 || j >= blocks->size() - 2)
						continue;
				auto& back_instr = block.instrs.back();
				if (back_instr.isJump())
					continue;
				// nojump
				auto next_label = blocks->at(j + 1).label;
				if (!hasMultiEntry(flow_graph, next_label)) {
					ForInstrs(k, blocks->at(j + 1).instrs, instr)
						blocks->at(j).instrs.push_back(instr);
					EndFor
					blocks->erase(blocks->begin() + j + 1);
					flag = true;
					ans = true;
				}
				EndFor
			} while (flag);
			return ans;
		}

		bool oneForAll() {
			bool ans = false;
			ForFuncs(i, func)
				FlowGraph flow_graph = constructFlowGraphFunc(func);

				ForBlocks(j, func.blocks, block)
					if (j != 0 && (hasNoEntry(flow_graph, block.label) || hasOnlySelfLoop(flow_graph, block.label))) {
						blocks->erase(blocks->begin() + j);
						flow_graph = constructFlowGraphFunc(func);
						ans = true;
						j--;
						continue;
					}
					if (!(block.instrs.empty() || j >= blocks->size() - 2) && block.instrs.back().type == Instr::JUMP && block.instrs.back().getJumpTarget() == blocks->at(j + 1).label && !hasMultiEntry(flow_graph, blocks->at(j + 1).label)) {
						block.instrs.back() = Instr(Instr::NOP);
						ForInstrs(k, blocks->at(j + 1).instrs, instr)
							blocks->at(j).instrs.push_back(instr);
						EndFor
							blocks->erase(blocks->begin() + j + 1);
						ans = true;
						flow_graph = constructFlowGraphFunc(func);
						continue;
					}
					if (!(block.instrs.size() == 0 || j >= blocks->size() - 2) && !block.instrs.back().isJump() && !hasMultiEntry(flow_graph, blocks->at(j + 1).label)) {
						ForInstrs(k, blocks->at(j + 1).instrs, instr)
							blocks->at(j).instrs.push_back(instr);
						EndFor
							blocks->erase(blocks->begin() + j + 1);
						ans = true;
						flow_graph = constructFlowGraphFunc(func);
						continue;;
					}
				
				EndFor
			EndFor
			endOneForAll : ;
			return ans;
		}
		
		bool blockMergeNoEntry() {
			bool ans = false;
			ForFuncs(i, func)
				if (blockMergeNoEntryFunc(func)) {
					ans = true;
				}
			EndFor
			return ans;
		}

		bool blockMergeNoEntryFunc(Func &func) {
			bool ans = false;
			bool flag;
			do {
				flag = false;
				FlowGraph flow_graph = constructFlowGraphFunc(func);

				ForBlocks(j, func.blocks, block)
					if (j == 0)
						continue;
					if (hasNoEntry(flow_graph, block.label) || hasOnlySelfLoop(flow_graph, block.label)) {
						blocks->erase(blocks->begin() + j);
						j--;
						flag = true;
						ans = true;
						continue;
					}
					
				EndFor
			} while (flag);
			return ans;
		}
		
#pragma endregion 


#pragma region LoopUnroll

		int maxLoopCnt() {
			int ans = -1;
			ForFuncs(i, func)
				ans = std::max(ans, maxLoopCntFunc(func));
			EndFor
			return ans;
		}

		int maxLoopCntFunc(Func &func) {
			int ans = -1;
			ForBlocks(k, func.blocks, block)
				string loop_name;
				int loop_block_index;
				tie(loop_block_index, loop_name) = findLoopName(func, k);
				if (loop_block_index == -9)
					continue;;

				int loop_endblock_index = findLoopEnd(func, loop_name);
				// debugln("loop_endblock_index: {}", loop_endblock_index);
				if (loop_endblock_index == -9)
					continue;

				int for_times = getLoopTimes(func, loop_endblock_index);
				// debugln("for_times: {}", for_times);

				if (for_times == -9)
					continue;
				ans = std::max(ans, for_times);
			EndFor
			return ans;
		}
		
		void loopUnroll() {
			ForFuncs(i, func)
				loopUnrollFunc(func);
			EndFor
		}

		int unroll_cnt = 0;

		void loopUnrollFunc(Func &func) {
			int factor = 10;
			// bool flag;
			// do {
			// 	flag = false;

				ForBlocks(k, func.blocks, block)
					// auto& blocks = func.blocks;
					if (k > 1000)
						break;

					string loop_name;
					int loop_block_index;
					tie(loop_block_index, loop_name) = findLoopName(func, k);
					if (loop_block_index == -9)
						continue;;

					int loop_endblock_index = findLoopEnd(func, loop_name);
					// debugln("loop_endblock_index: {}", loop_endblock_index);
					if (loop_endblock_index == -9)
						continue;

					int for_times = getLoopTimes(func, loop_endblock_index);
					// debugln("for_times: {}", for_times);

					if (for_times == -9)
						continue;
					factor = 10;
					auto& jump_instr = blocks->at(loop_endblock_index).instrs.back();
					if (for_times <= factor || !isNumber(jump_instr.source_a)) {

						// Unroll
						vector<Block> loop_blocks;
						for (int i = loop_block_index; i <= loop_endblock_index; i++) {
							loop_blocks.push_back(blocks->at(loop_block_index));
							blocks->erase(blocks->begin() + loop_block_index);
						}
						loop_blocks.back().instrs.pop_back();
						// Block loop_block = func.blocks->at(loop_block_index);
						// loop_block.instrs.pop_back();
						//
						// func.blocks->erase(func.blocks->begin() + loop_block_index);
						//
						vector<string> block_names;
						for (int i = 0; i < loop_blocks.size(); i++) {
							block_names.push_back(loop_blocks[i].label);
						}

						for (int i = 0; i < for_times; i++) {
							unroll_cnt++;
							// change name
							vector<Block> new_blocks = loop_blocks;
							for (int j = 0; j < loop_blocks.size(); j++) {
								changeJumpInBlocks(new_blocks, new_blocks[j].label, getUnrollName(block_names[j]));
								new_blocks[j].label = getUnrollName(block_names[j]);
							}
							// insert
							for (int j = new_blocks.size() - 1; j >= 0; j--) {
								blocks->insert(blocks->begin() + loop_block_index, new_blocks[j]);
							}
						}
					} else {

						
						
						// Unroll
						vector<Block> loop_blocks;
						for (int i = loop_block_index; i <= loop_endblock_index; i++) {
							loop_blocks.push_back(blocks->at(i));
							// blocks->erase(blocks->begin() + loop_block_index);
						}
						loop_blocks.back().instrs.pop_back();

						auto& jump_instr = blocks->at(loop_endblock_index).instrs.back();
						if (isNumber(jump_instr.source_a)) {
							jump_instr.source_a = i2a(a2i(jump_instr.source_a) - factor);
						}
						else {
							string source_a = jump_instr.source_a;
							Instr sub_instr = Instr(Instr::MINUS, FORMAT("{}_unroll", source_a), source_a, factor);
							auto& end_instrs = blocks->at(loop_endblock_index).instrs;
							end_instrs.insert(end_instrs.end() - 1, sub_instr);
							auto& jump_instr = blocks->at(loop_endblock_index).instrs.back();
							jump_instr.source_a = FORMAT("{}_unroll", source_a);
						}
						// Block loop_block = func.blocks->at(loop_block_index);
						// loop_block.instrs.pop_back();
						//
						// func.blocks->erase(func.blocks->begin() + loop_block_index);
						//
						vector<string> block_names;
						for (int i = 0; i < loop_blocks.size(); i++) {
							block_names.push_back(loop_blocks[i].label);
						}

						for (int i = 0; i < factor; i++) {
							unroll_cnt++;
							// change name
							vector<Block> new_blocks = loop_blocks;
							for (int j = 0; j < loop_blocks.size(); j++) {
								changeJumpInBlocks(new_blocks, new_blocks[j].label, getUnrollName(block_names[j]));
								new_blocks[j].label = getUnrollName(block_names[j]);
							}
							// insert
							for (int j = new_blocks.size() - 1; j >= 0; j--) {
								blocks->insert(blocks->begin() + loop_block_index, new_blocks[j]);
							}
						}
					}
			


					return;

				EndFor
				
				

				
			// } while (flag);

		}

		string getUnrollName(string origin_name) {
			return FORMAT("{}_unroll_{}", origin_name, unroll_cnt);
		}

		int findLoopEnd(Func &func, string loop_name) {
			ForBlocks(j, func.blocks, block)
				Instr jump_instr = getJumpInstr(block);
				if (jump_instr.isJump() && jump_instr.getJumpTarget() == loop_name)
					return j;
			EndFor
			return -9;
		}

		tuple<int, string> findLoopName(Func &func, int block_index) {
			// ForBlocks(j, func.blocks, block)
			auto& block = func.blocks->at(block_index);
				if (starts_with(block.label, string("for_body")) 
					|| starts_with(block.label, string("while_body"))
					)
				{
					string loop_name = block.label;
					return make_tuple(block_index, loop_name);
				}
			// EndFor
			return make_tuple(-9, "None");
		}

		int getLoopTimes(Func &func, int for_block_index) {
			InstrIterator i(func, for_block_index, func.blocks->at(for_block_index).instrs.size() - 1);
			// i.previous();
			int loop_times = -9;

			Instr loop_branch_instr = i.getInstr();
			string loop_var = i.getInstr().target;
			
			// debugln("loop_var: {}", loop_var);

			int loop_end;
			if (i.getInstr().has_source_a_value) {
				loop_end = i.getInstr().source_a_value;
			} else {
				loop_end = getLoopEnd(func, i, i.getInstr().source_a);
			}
			// debugln("loop_start: {}", loop_end);
			
			//
			string loop_start_var;
			while (i.previous()) {
				if ((i.getInstr().type == Instr::PLUS && i.getInstr().target == loop_var)) {
					if (i.getInstr().source_b_value != 1)
						break;
					loop_start_var = i.getInstr().source_a;
					break;
				}
			}
			// debugln("loop_start_var: {}", loop_start_var);

			int loop_start = -9;
			while (i.previous()) {
				if (i.getInstr().type == Instr::LI && i.getInstr().target == loop_start_var) {
					loop_start = i.getInstr().source_a_value;
					break;
				}
			}
			// debugln("loop_start: {}", loop_start);

			if (loop_start != -9 && loop_end != -9) {
				switch (loop_branch_instr.type) {
				case Instr::BLT:
					loop_times = loop_end - loop_start;
					break;
				case Instr::BLE:
					loop_times = loop_end - loop_start + 1;
				default:;
				}
				
			}
			
			return loop_times;
		}

		int getLoopEnd(Func &func, InstrIterator i, string source_a) {
			int end = -9;
			while (i.previous()) {
				if (i.getInstr().type == Instr::LI && i.getInstr().target == source_a) {
					end = a2i(i.getInstr().source_a);
					break;
				}
				if (i.getInstr().getStoreName() == source_a)
					break;
			}
			return end;
		}
		
#pragma endregion 
		
	};
	
}
