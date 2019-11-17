#pragma once

#include "../meow/core.hpp"
#include "global_define.hpp"
#include "block.hpp"
#include "func.hpp"
#include <memory>

namespace buaac {

	using global::GlobalDefine;

	
	class IR {
	public:


		std::vector<GlobalDefine> global_defines;
		std::vector<Func> funcs{};
		BlocksPtr main_blocks;

		int cnt_c = 0;
		std::string func_name;
		
		BlocksPtr blocks_now;

		IR() {
			main_blocks = std::make_shared<Blocks>();
			main_blocks->emplace_back(Block("main"));
			blocks_now = nullptr;
		}

		void defineMain() {
			blocks_now = main_blocks;
			func_name = "main";
		}

		void defineFunc(std::string func_name) {
			funcs.emplace_back(Func(func_name));
			blocks_now = funcs.back().blocks;
			this->func_name = func_name;
		}

		std::string getReturnLabel() {
			return FORMAT("return_{}", func_name);
		}
		
		void newBlock(std::string label) {
			blocks_now->emplace_back(Block(label));
		}

		Blocks& getMainBlocks() {
			return *main_blocks;
		}

		void printLine() {
			appendInstr(Instr::PRINT_LINE);
		}

		void defineGlobalInt(std::string ident) {
			globalDefine(global::VarInt{ getGlobalName(ident) });
		}
		
		std::string defineConstStr(std::string str) {
			std::string str_label = newConstLabel();
			globalDefine(global::ConstStr{ str_label, str });
			return str_label;
		}
		
		void printGlobalStr(std::string label) {
			appendInstr({ Instr::PRINT_GLOBAL_STR, label });
		}
		
		void scanf(int scope, syntax::Symbol symbol) {
			if (scope == 0) {
				appendInstr({Instr::SCAN_GLOBAL_INT, getGlobalName(symbol.getIdent()) });
			}
		}

		void printInt(std::string reg) {
			appendInstr({Instr::PRINT_INT, reg});
		}

		void pushStackReg(std::string reg) {
			appendInstr(Instr(Instr::PUSH_REG, reg));
		}

		void popStackReg(std::string reg) {
			appendInstr(Instr(Instr::POP_REG, reg));
		}

		void popStack(size_t size) {
			appendInstr(Instr(Instr::POP, size));
		}

		void call(const std::string& func_name) {
			appendInstr(Instr(Instr::CALL, func_name));
		}

		void moveReg(const std::string target, const std::string& source) {
			appendInstr({ Instr::PLUS, target, "$0", source });
		}

		

		enum {
			DEFINE_IF,
			DEFINE_FOR,
		} jump_define;
		
		void newIf() {
			if_cnt++;
			jump_define = DEFINE_IF;
		}

		std::string getCondJumpName() {
			switch (jump_define) {

			case DEFINE_IF:
				return getIfElseName();
				break;
			case DEFINE_FOR:
				return getForEndName();
				break;
			default: ;
			}
		}

		std::string getIfName() {
			return FORMAT("if_{}", if_cnt);
		}

		std::string getIfThanName() {
			return FORMAT("if_{}_than", if_cnt);
		}
		
		std::string getIfElseName() {
			return FORMAT("if_{}_else", if_cnt);
		}

		std::string getIfEndName() {
			return FORMAT("if_{}_end", if_cnt);
		}

		void jump(const std::string& label) {
			appendInstr(Instr(Instr::JUMP, label));
		}

		void newFor() {
			for_cnt++;
			jump_define = DEFINE_FOR;
		}

		std::string getForStartName() {
			return FORMAT("for_{}_start", for_cnt);
		}

		std::string getForBodyName() {
			return FORMAT("for_{}_body", for_cnt);
		}

		std::string getForEndName() {
			return FORMAT("for_{}_end", for_cnt);
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

		std::vector< std::vector<OP>> op_stack_stack{};
		std::vector< std::vector<std::string> > obj_stack_stack{};
		std::vector< std::vector<Instr> > instrs_stack{};
		
		std::vector<OP> op_stack;
		std::vector<std::string> obj_stack;
		std::vector<Instr> instrs;

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

		void exprPushGlobalVar(std::string ident) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_LAB, t, getGlobalName(ident)));
			obj_stack.push_back(t);
		}

		void exprPushStackVar(std::string ident, int bytes) {
			auto t = newTemp();
			appendInstr(Instr(Instr::LOAD_STA, t, i2a(bytes)));
			obj_stack.push_back(t);
		}

		void exprPushLiteralInt(std::string value) {
			auto t = newTemp();
			appendInstr(Instr(Instr::PLUS, t, "$0", value));
			obj_stack.push_back(t);
		}

		void exprPushLiteralInt(int value) {
			exprPushLiteralInt(i2a(value));
		}

		int not_gen = 0;

		void notGen() {
			not_gen++;
		}


		
		void exprStart() {
			instrs_stack.push_back(instrs);
			op_stack_stack.push_back(op_stack);
			obj_stack_stack.push_back(obj_stack);
			
			instrs.clear();
			op_stack.clear();
			obj_stack.clear();
		}
		
		std::string gen() {
			if (not_gen > 0) {
				not_gen--;
				return "NOTGEN";
			}
			while (!op_stack.empty()) {
				pop_op();
			}
			std::string ans = obj_stack.back();
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
			std::string source_a, source_b;
			std::string temp;
			
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
				instrs.emplace_back(Instr(Instr::MINUS, newTemp(), "0", source_b));
				obj_stack.push_back(temp);
				break;
			default:;
			}
			op_stack.pop_back();
		}
		

#pragma endregion 

		void pushStackVars(int bytes) {
			appendInstr(Instr(Instr::PUSH, bytes));
		}

		void saveStack(std::string reg, int bytes) {
			appendInstr(Instr(Instr::SAVE_STA, reg, bytes));
		}

		void loadStack(std::string reg, int bytes) {
			appendInstr(Instr(Instr::LOAD_STA, reg, bytes));
		}

		// std::ofstream fout;
		
		// Output
		void output(std::string filename) {
			// fout.open(filename);
			for (int i = 0; i < blocks_now->size(); i++) {
				output(blocks_now->at(i));
			}
		}
		
		void output(const Block& block) {
			
		}

		void appendInstr(Instr instr) {
			blocks_now->back().addInstr(instr);
		}
		
	private:

		int temp_cnt = 0;
		int if_cnt = 0;
		int for_cnt = 0;

		std::string newTemp() {
			return FORMAT("__T{}", temp_cnt++);
		}

		enum {
			DEFINE_MAIN,
			DEFINE_FUNC,
		};
		
		

		void globalDefine(GlobalDefine global_define) {
			global_defines.emplace_back(global_define);
		}

		std::string newConstLabel() {
			return FORMAT("__CONST_{}", cnt_c++);
		}

		std::string getGlobalName(std::string ident) {
			return FORMAT("__GLOBAL_{}", ident);
		}
	};
	
}

