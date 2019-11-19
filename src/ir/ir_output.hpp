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

		void outputGloablDefine(GlobalDefine& global_define) {
			switch (global_define.type) {

			case GlobalDefine::CONST_STR:
				write("const string {} = {};", global_define.const_str.label, global_define.const_str.value);
				break;
			case GlobalDefine::VAR_INT:
				write("int {};", global_define.var_int.label);
				break;
			case GlobalDefine::VAR_INT_ARR:
				write("int {}[{}];", global_define.var_int_arr.label, global_define.var_int_arr.len);
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
			for (int i = 0; i < block.instrs.size(); i++) {
				outputInstr(block.instrs[i]);
			}
		}

		void outputInstr(Instr &instr) {
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
				write("print('\n');");
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
			case Instr::LA:
				write("{} = {};", instr.target, instr.source_a);
				break;
			case Instr::PUSH: break;
			case Instr::POP: break;
			case Instr::PUSH_REG: break;
			case Instr::POP_REG: break;
			case Instr::LOAD_LAB: break;
			case Instr::LOAD_LAB_IMM: break;
			case Instr::LOAD_STA: break;
			case Instr::SAVE_LAB: break;
			case Instr::SAVE_LAB_IMM: break;
			case Instr::SAVE_STA: break;
			case Instr::SAVE_STA_ARR: break;
			case Instr::LOAD_STA_ARR: break;
			case Instr::CALL: break;
			case Instr::RETURN_END: break;
			case Instr::JUMP: break;
			case Instr::BGEZ: break;
			case Instr::BLEZ: break;
			case Instr::BLT: break;
			case Instr::BGT: break;
			case Instr::BLE: break;
			case Instr::BGE: break;
			case Instr::BEQ: break;
			case Instr::BNE: break;
			default: ;
			}
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

			write("fn main {}", '{');
			pushIndent();
			for (int i = 0; i < ir.main_blocks->size(); i++) {
				outputBlock(ir.main_blocks->at(i));
			}
			popIndent();
			write("{}", '}');
			write("");
		}

		
		
	};
}