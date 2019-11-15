#pragma once

#include "../meow/core.hpp"
#include "global_define.hpp"
#include "block.hpp"

namespace buaac {

	using global::GlobalDefine;

	
	class IR {
	public:

		std::vector<GlobalDefine> global_defines;
		std::vector<Block> blocks;

		int cnt_c = 0;
		

		

		void newBlock(std::string label) {
			blocks.emplace_back(Block(label));
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

		std::vector<OP> op_stack;
		std::vector<std::string> obj_stack;
		std::vector<Instr> instrs;

		void push(OP op) {
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

		

		std::string gen() {
			while (!op_stack.empty()) {
				pop_op();
			}
			std::string ans = obj_stack.back();
			obj_stack.pop_back();
			assert(obj_stack.empty());
			for (int i = 0; i < instrs.size(); i++) {
				appendInstr(instrs[i]);
			}
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
			appendInstr(Instr(Instr::PUSH, i2a(bytes)));
		}

		void saveStack(std::string reg, int bytes) {
			appendInstr(Instr(Instr::SAVE_STA, reg, i2a(bytes)));
		}

		void loadStack(std::string reg, int bytes) {
			appendInstr(Instr(Instr::LOAD_STA, reg, i2a(bytes)));
		}

		// std::ofstream fout;
		
		// Output
		void output(std::string filename) {
			// fout.open(filename);
			for (int i = 0; i < blocks.size(); i++) {
				output(blocks[i]);
			}
		}

		void output(const Block& block) {
			
		}
		
	private:

		int temp_cnt = 0;

		std::string newTemp() {
			return FORMAT("__T{}", temp_cnt++);
		}
		
		void appendInstr(Instr instr) {
			blocks.back().addInstr(instr);
		}

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

