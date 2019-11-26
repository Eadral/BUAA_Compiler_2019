#pragma once
#include "../meow/core.hpp"
#include "ir.hpp"

namespace buaac {
	class IrGen {

		IR ir;

		std::ofstream fout;

		int output_offset = 0;

		std::string outputOffset() {
			std::string offset = "";
			for (int i = 0; i < output_offset; i++) {
				offset += " ";
			}
			return offset;
		}

		void pushIndent() {
			output_offset += 2;
		}

		void popIndent() {
			output_offset -= 2;
		}

		
#define write(...)	\
	fout << outputOffset() << FORMAT(__VA_ARGS__) << std::endl

#define writeNoOffset(...)	\
	fout << FORMAT(__VA_ARGS__) << std::endl

		void outputGloablDefine(GlobalDefine& global_define) {
			switch (global_define.type) {

			case GlobalDefine::CONST_STR:
				write("const string {} = \"{}\";", removePrefix(global_define.const_str.label), global_define.const_str.value);
				break;
			case GlobalDefine::VAR_INT:
				write("int {};", removePrefix(global_define.var_int.label));
				break;
			case GlobalDefine::VAR_INT_ARR:
				write("int {}[{}];", removePrefix(global_define.var_int_arr.label), global_define.var_int_arr.len);
				break;
			default: ;
			}
		}

		void outputFunc(Func &func) {
			write("fn {} {}", func.func_name, '{');
			pushIndent();
			for (int i = 0; i < func.blocks->size(); i++) {
				outputBlock(func.blocks->at(i));
			}
			popIndent();
			write("{}", '}');
		}

		void outputBlock(Block &block) {
			writeNoOffset("{}:", block.label);
			for (int i = 0; i < block.instrs.size(); i++) {
				outputInstr(block.instrs[i]);
			}
		}

		void outputInstr(Instr instr) {
			if (instr.show == false)
				return;
			bool show_as = instr.showas.size() != 0;
			// if (instr.show_a.size() == 0)
			instr.target = removePrefix(instr.target);
			// if (instr.show_b.size() == 0)
			instr.source_a = removePrefix(instr.source_a);
			// if (instr.show_c.size() == 0)
			instr.source_b = removePrefix(instr.source_b);
			
			switch (instr.type) {

			case Instr::PRINT_GLOBAL_STR:
			case Instr::PRINT_INT:
			case Instr::PRINT_CHAR:
				write("print({});", instr.target);
				break;
			case Instr::SCAN_INT: 
			case Instr::SCAN_CHAR: 
			case Instr::SCAN_GLOBAL_INT: 
			case Instr::SCAN_GLOBAL_CHAR:
				write("scanf({});", instr.target);
				break;
			case Instr::PRINT_LINE:
				write("print('\\n');");
				break;
			case Instr::PLUS:
				write("{} = {} + {};", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MINUS:
				write("{} = {} - {};", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MULT:
				write("{} = {} * {};", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::DIV:
				write("{} = {} / {};", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::MOVE:
				write("{} = {};", instr.target, instr.source_a);
				break;
			case Instr::LI:
				write("{} = {};", instr.target, instr.source_a);
				break;
			case Instr::LA:
				write("{} = {};", instr.target, instr.source_a);
				break;
			case Instr::PUSH:
				write("push {}B;", instr.target);
				break;
			case Instr::POP:
				write("pop {}B;", instr.target);
				break;
			case Instr::PUSH_REG:
				write("push {};", instr.target);
				break;
			case Instr::POP_REG:
				write("pop {};", instr.target);
				break;
			case Instr::LOAD_GLO:
				if (show_as) {
					write("{} = {};", instr.target, instr.showas);
					break;
				}
				write("{} = $gp[{}];", instr.target, instr.source_a);
				break;
			// case Instr::LOAD_LAB_IMM:
			// 	write("{} = {}[{}];", instr.target, instr.source_a, instr.source_b);
			// 	break;
			case Instr::LOAD_STA:
				if (show_as) {
					write("{} = {};", instr.target, instr.showas);
					break;
				}
				write("{} = $sp[{}];", instr.target, instr.source_a);
				break;
			case Instr::SAVE_GLO:
				if (show_as) {
					write("{} = {};", instr.showas, instr.target);
					break;
				}
				write("$gp[{}] = {};", instr.source_a, instr.target);
				break;
			// case Instr::SAVE_LAB_IMM:
			// 	// if (show_as) {
			// 	// 	write("{} = {}", instr.target, instr.showas);
			// 	// }
			// 	write("{}[{}] = {};",  instr.source_a, instr.source_b, instr.target);
			// 	break;
			case Instr::SAVE_STA:
				if (show_as) {
					write("{} = {};", instr.showas, instr.target);
					break;
				}
				write("$sp[{}] = {};", instr.source_a, instr.target);
				break;
			case Instr::SAVE_STA_ARR:
				write("[{}] = {};", instr.source_a, instr.target);
				break;
			case Instr::LOAD_STA_ARR:
				write("{} = [{}];", instr.target, instr.source_a);
				break;
			case Instr::CALL:
				write("call {};", instr.target);
				break;
			case Instr::RETURN_END:
				write("return {};", instr.target);
				break;
			case Instr::JUMP:
				write("jump {};", instr.target);
				break;
			case Instr::BGEZ:
				write("if ({} >= 0) jump {}", instr.target, instr.source_a);
				break;
			case Instr::BLEZ:
				write("if ({} <= 0) jump {}", instr.target, instr.source_a);
				break;
			case Instr::BLT:
				write("if ({} < {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGT:
				write("if ({} > {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BLE:
				write("if ({} <= {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BGE:
				write("if ({} >= {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BEQ:
				write("if ({} == {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::BNE:
				write("if ({} != {}) jump {}", instr.target, instr.source_a, instr.source_b);
				break;
			case Instr::IR_SHOW:
				write("{}", instr.target);
				break;
			case Instr::PUSH_REGPOOL:
				write("push regpool;");
				break;
			case Instr::POP_REGPOOL:
				write("pop regpool");
				break;
			default: ;
			}
		}

		std::string removePrefix(std::string input) {
			if (input == "$v0")
				return "RET";
			if (input.size() >= 3) {
				if (input[0] == '_' && input[2] == 'G') {
					return input.substr(9);
				}
				if (input[0] == '_') 
					return input.substr(2);
			}
			return input;
		}
		
	public:
		
		IrGen(IR ir): ir(ir) {}

		void output(std::string filename) {
			fout.open(filename);

			write("// BUAAC IR, Build: {}, qaq", __TIMESTAMP__);
			write("");
			
			for (int i = 0; i < ir.global_defines.size(); i++) {
				outputGloablDefine(ir.global_defines[i]);
			}
			write("");
			
			for (int i = 0; i < ir.funcs.size(); i++) {
				outputFunc(ir.funcs[i]);
				write("");
			}

			// write("fn main {}", '{');
			// pushIndent();
			// for (int i = 0; i < ir.main_blocks->size(); i++) {
			// 	outputBlock(ir.main_blocks->at(i));
			// }
			// popIndent();
			// write("{}", '}');
			// write("");
		}

		
		
	};
}