#pragma once

#include "../meow/core.hpp"
#include "global_define.hpp"
#include "block.hpp"
#include "func.hpp"
#include <memory>
#include "var_range.hpp"

// #define POOLSIZE 30

namespace buaac {

	using global::GlobalDefine;

	
	class IR {
	public:

		map<string, map<string, VarRange>> func_to_ident_to_range;

		vector<GlobalDefine> global_defines;
		vector<Func> funcs{};
		// BlocksPtr main_blocks;

		int cnt_c = 0;
		string func_name;
		
		BlocksPtr blocks_now;

		IR() {
			// main_blocks = std::make_shared<Blocks>();
			// main_blocks->emplace_back(Block("main"));
			blocks_now = nullptr;
		}

		// void defineMain() {
		// 	temp_cnt = 0;
		// 	// blocks_now = main_blocks;
		// 	// func_name = "main";
		// }

		void defineFunc(string func_name) {
			// temp_cnt = 0;
			funcs.emplace_back(Func(func_name));
			blocks_now = funcs.back().blocks;
			this->func_name = func_name;
		}

		string getReturnLabel() {
			return FORMAT("return_{}", func_name);
		}
		
		void newBlock(string label) {
			blocks_now->emplace_back(Block(label));
		}

		// Blocks& getMainBlocks() {
		// 	return *main_blocks;
		// }

		void printLine() {
			appendInstr(Instr::PRINT_LINE);
		}

		void defineGlobalInt(string ident) {
			globalDefine(global::VarInt{ (ident) });
		}
		
		string defineConstStr(string str) {
			string str_label = newConstLabel();
			globalDefine(global::ConstStr{ str_label, str });
			return str_label;
		}
		
		void printGlobalStr(string label) {
			appendInstr({ Instr::PRINT_GLOBAL_STR, label });
		}
		
		// void scanf(int scope, syntax::Symbol symbol) {
		// 	if (scope == 0) {
		// 		appendInstr({Instr::SCAN_GLOBAL_INT, getGlobalName(symbol.getIdent()) });
		// 	}
		// }

		// void printInt(string reg) {
		// 	appendInstr({Instr::PRINT_INT, reg});
		// }

		// void pushStackReg(string reg) {
		// 	appendInstr(Instr(Instr::PUSH_REG, reg));
		// }
		//
		// void popStackReg(string reg) {
		// 	appendInstr(Instr(Instr::POP_REG, reg));
		// }

		void __pushStackReg(string reg) {
			appendInstr(Instr(Instr::PUSH_REG, reg));
		}

		void __popStackReg(string reg) {
			appendInstr(Instr(Instr::POP_REG, reg));
		}

		void pushStackVars(int bytes) {
			if (bytes == 0)
				return;
			appendInstr(Instr(Instr::PUSH, bytes));
			instrNotShow();
		}
		
		void popStack(size_t size) {
			if (size == 0)
				return;
			appendInstr(Instr(Instr::POP, size));
			instrNotShow();
		}

		void call(const string& func_name) {
			appendInstr(Instr(Instr::CALL, func_name));
		}

		void moveReg(const string target, const string& source) {
			appendInstr({ Instr::MOVE, target,  source });
		}

		void defineGlobalIntArr(const string& ident, int len) {
			globalDefine(global::VarIntArr{ (ident), len });
		}


		enum JumpDefine {
			DEFINE_IF,
			DEFINE_FOR,
			DEFINE_WHILE,
			DEFINE_DOWHILE,
		} ;

		vector<JumpDefine> jump_define_stack{};
		vector<int> jump_cnt_stack{};
		int jump_cnt = 0;
		// vector<string> label_stack{};

		void endJumpDefine() {
			jump_define_stack.pop_back();
			jump_cnt_stack.pop_back();
			// label_stack.pop_back();
		}

		void newJumpCnt() {
			jump_cnt++;
			jump_cnt_stack.push_back(jump_cnt);
		}
		
		void newIf() {
			newJumpCnt();
			jump_define_stack.push_back(DEFINE_IF);
		}

		string getCondJumpName() {
			switch (jump_define_stack.back()) {

			case DEFINE_IF:
				return getIfElseName();
				break;
			case DEFINE_FOR:
				return getForEndName();
				break;
			case DEFINE_WHILE:
				return getWhileEndName();
				break;
			case DEFINE_DOWHILE:
				return getDoWhileEndName();
				break;
			default: ;
			}
		}

		string getIfName() {
			return FORMAT("if_{}", jump_cnt_stack.back());
		}

		string getIfThanName() {
			return FORMAT("if_{}_than", jump_cnt_stack.back());
		}
		
		string getIfElseName() {
			return FORMAT("if_{}_else", jump_cnt_stack.back());
		}

		string getIfEndName() {
			return FORMAT("if_{}_end", jump_cnt_stack.back());
		}

		void jump(const string& label) {
			appendInstr(Instr(Instr::JUMP, label));
		}

		void newFor() {
			newJumpCnt();
			jump_define_stack.push_back(DEFINE_FOR);
		}

		string getForStartName() {
			return FORMAT("for_{}_start", jump_cnt_stack.back());
		}

		string getForBodyName() {
			return FORMAT("for_{}_body", jump_cnt_stack.back());
		}

		string getForEndName() {
			return FORMAT("for_{}_end", jump_cnt_stack.back());
		}

		void newWhile() {
			newJumpCnt();
			jump_define_stack.push_back(DEFINE_WHILE);
		}

		string getWhileName() {
			return FORMAT("while_{}", jump_cnt_stack.back());
		}

		string getWhileEndName() {
			return FORMAT("while_{}_end", jump_cnt_stack.back());
		}

		void newDoWhile() {
			newJumpCnt();
			jump_define_stack.push_back(DEFINE_DOWHILE);
		}

		string getDoWhileName() {
			return FORMAT("dowhile_{}", jump_cnt_stack.back());
		}

		string getDoWhileEndName() {
			return FORMAT("dowhile_{}_end", jump_cnt_stack.back());
		}

#pragma region expr

		enum OP {
			PARE_L = 0,
			PARE_R,
			PLUS,
			MINUS,
			MULT,
			DIV,
			NEG,
		};
		const int priority[10] = { 0, 0, 1, 1, 2, 2, 3 };

		vector< vector<OP>> op_stack_stack{};
		vector< vector<string> > obj_stack_stack{};
		vector< vector<Instr> > instrs_stack{};
		
		vector<OP> op_stack;
		vector<string> obj_stack;
		vector<Instr> instrs;

		void exprPush(OP op) {
			switch (op) {
			case PARE_L:
				op_stack.push_back(op);
				break;
			case PARE_R:
				while (op_stack.back() != PARE_L) {
					pop_op();
				}
				op_stack.pop_back();
				break;
			case PLUS:
			case MINUS:
			case MULT:
			case DIV:
			case NEG:
				while (pop_first(op))
					pop_op();
				op_stack.push_back(op);
				break;
			default:;
			}
		}

		void exprPushGlobalVar(string ident, int bytes) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_GLO, t, bytes));
			instrShowAs(getGlobalName(ident));	
			obj_stack.push_back(t);
		}

		void exprPushStackVar(string ident, int bytes) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_STA, t, bytes));
			instrShowAs(ident);
			obj_stack.push_back(t);
		}

		void exprPushStackArrGlo(int arr, string offset) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_ARR_GLO, t, arr, offset));
			obj_stack.push_back(t);
		}

		void exprPushStackArrSta(int arr, string offset) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_ARR_STA, t, arr, offset));
			obj_stack.push_back(t);
		}

		void exprPushLiteralInt(int value) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LI, t, value));
			obj_stack.push_back(t);
		}

		void exprPushReg(string reg) {
			auto t = newTemp();
			appendInstr(Instr(Instr::MOVE, t, reg));
			obj_stack.push_back(t);
		}

		void exprPushLabel(string label) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LA, t, label));
			obj_stack.push_back(t);
		}

		// void exprPushLiteralInt(int value) {
		// 	exprPushLiteralInt(value);
		// }

		int not_gen = 0;

		// void notGen() {
		// 	not_gen++;
		// }


		
		void exprStart() {
			instrs_stack.push_back(instrs);
			op_stack_stack.push_back(op_stack);
			obj_stack_stack.push_back(obj_stack);
			
			instrs.clear();
			op_stack.clear();
			obj_stack.clear();
		}
		
		string gen() {
			// if (not_gen > 0) {
			// 	not_gen--;
			// 	return "NOTGEN";
			// }
			while (!op_stack.empty()) {
				pop_op();
			}
			string ans = obj_stack.back();
			obj_stack.pop_back();
			assert(obj_stack.empty());
			for (int i = 0; i < instrs.size(); i++) {
				appendInstr(instrs[i]);
			}

			instrs = instrs_stack.back();
			instrs_stack.pop_back();
			op_stack = op_stack_stack.back();
			op_stack_stack.pop_back();
			obj_stack = obj_stack_stack.back();
			obj_stack_stack.pop_back();
			
			return ans;
		}

		bool pop_first(OP op) {
			if (!op_stack.empty() && priority[op_stack.back()] >= priority[op]) {
				return true;
			}
			else {
				return false;
			}
		}

#define DOBLE_OP(OP)	\
		temp = newTemp();	\
		source_b = obj_stack.back();	\
		obj_stack.pop_back();	\
		source_a = obj_stack.back();	\
		obj_stack.pop_back();	\
		instrs.emplace_back(Instr(Instr::OP, temp, source_a, source_b));	\
		obj_stack.push_back(temp);
		
		void pop_op() {
			string source_a, source_b;
			string temp;
			
			switch (op_stack.back()) {

			case PARE_L: break;
			case PARE_R: break;
			case PLUS:
				DOBLE_OP(PLUS)
				break;
			case MINUS:
				DOBLE_OP(MINUS)
				break;
			case MULT:
				DOBLE_OP(MULT)
				break;
			case DIV:
				DOBLE_OP(DIV)
				break;
			case NEG:
				temp = newTemp();
				source_b = obj_stack.back();
				obj_stack.pop_back();
				// source_a = obj_stack.back();
				// obj_stack.pop_back();
				instrs.emplace_back(Instr(Instr::MINUS, temp, "$0", source_b));
				obj_stack.push_back(temp);
				break;
			default:;
			}
			op_stack.pop_back();
		}
		

#pragma endregion 


		// void saveStack(string reg, int bytes) {
		// 	appendInstr(Instr(Instr::SAVE_STA, reg, bytes));
		// }
		//
		// void saveLabel(string label) {
		// 	appendInstr({ Instr::SAVE_LAB, label });
		// }

		// void loadStack(string reg, int bytes) {
		// 	appendInstr(Instr(Instr::LOAD_STA, reg, bytes));
		// }

		void irShow(string str) {
			appendInstr({ Instr::IR_SHOW,  str });
		}

		

		void appendInstr(Instr instr, bool show=true) {
			instr.show = show;
			blocks_now->back().addInstr(instr);
		}

		void instrNotShow() {
			// blocks_now->back().instrs.back().show = false;
		}

		void instrShowAs(string showas) {
			blocks_now->back().instrs.back().showas = showas;
		}
		
		// string getGlobalName(string ident) {
		// 	return FORMAT("__GLOBAL_{}", ident);
		// }

		int newLabelCnt() {
			return label_cnt++;
		}
		
	private:

		int temp_cnt = 0;
		int label_cnt = 0;

		

		string newTemp() {
			return FORMAT("__T{}", temp_cnt++);
		}

		enum {
			DEFINE_MAIN,
			DEFINE_FUNC,
		};
		
		

		void globalDefine(GlobalDefine global_define) {
			global_defines.emplace_back(global_define);
		}

		string newConstLabel() {
			return FORMAT("__CONST_{}", cnt_c++);
		}

		
	};
	
}

#define ForFuncs(i, func)	 for (int i = 0; i < ir.funcs.size(); i++) { auto& funcs = ir.funcs; auto& func = ir.funcs.at(i);

#define ForBlocks(j, _blocks, block) for (int j = 0; j < _blocks->size(); j++) { auto& blocks = _blocks; auto& block = _blocks->at(j);

#define ForInstrs(k, _instrs, instr) for (int k = 0; k < _instrs.size(); k++) { auto& instrs = _instrs; auto& instr = _instrs[k]; 

