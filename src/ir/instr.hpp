#pragma once

#include "../meow/core.hpp"

namespace buaac {
	namespace instr {

		class Instr {
		public:
			enum Type {
				// Label
				PRINT_GLOBAL_STR,
				PRINT_INT,
				PRINT_CHAR,
				SCAN_INT,
				SCAN_CHAR,
				SCAN_GLOBAL_INT,
				SCAN_GLOBAL_CHAR,

				// Empty
				PRINT_LINE,

				// ALU
				PLUS,
				MINUS,
				MULT,
				DIV,
				LA,
				MOVE,
				LI,

				// Stack
				PUSH,
				POP,
				PUSH_REG,
				POP_REG,
				PUSH_REGPOOL,
				POP_REGPOOL,

				// Memory
				LOAD_GLO,
				LOAD_STA,
				SAVE_GLO,
				SAVE_STA,
				SAVE_ARR_GLO,
				LOAD_ARR_GLO,
				SAVE_ARR_STA,
				LOAD_ARR_STA,

				// JUMP
				CALL,
				RETURN_END,
				JUMP,
				BGEZ,
				BLEZ,
				BGTZ,
				BLT,
				BGT,
				BLE,
				BGE,
				BEQ,
				BNE,
				NOP,

				// IR
				IR_SHOW,

				PARA,
				VAR,

			} type;

			
			string target{}, source_a{}, source_b{};
			int target_value, source_a_value, source_b_value;
			bool has_target_value = false, has_source_a_value = false, has_source_b_value = false;
			int ans;
			bool has_ans = false;
			bool show = true;
			string showas;
			int block_line_number=-1;

			Instr(Type type) : type(type) { init(); }
			Instr(Type type, string target)
				: type(type), target(target) {
				init();
			}
			Instr(Type type, string target, string source_a)
				: type(type), target(target), source_a(source_a) {
				init();
			}
			Instr(Type type, string target, string source_a, string source_b)
				: type(type), target(target), source_a(source_a), source_b(source_b) {
				init();
			}

			Instr(Type type, int target)
				: type(type), target(i2a(target)) {
				init();
			}
			Instr(Type type, int target, string source_a)
				: type(type), target(i2a(target)), source_a(source_a) {
				init();
			}
			Instr(Type type, string target, int source_a)
				: type(type), target(target), source_a(i2a(source_a)) {
				init();
			}
			Instr(Type type, int target, string source_a, string source_b)
				: type(type), target(i2a(target)), source_a(source_a), source_b(source_b) {
				init();
			}
			Instr(Type type, string target, int source_a, string source_b)
				: type(type), target(target), source_a(i2a(source_a)), source_b(source_b) {
				init();
			}
			Instr(Type type, string target, string source_a, int source_b)
				: type(type), target(target), source_a(source_a), source_b(i2a(source_b)) {
				init();
			}

			void init() {
				if (!has_target_value && isNumber(target)) {
					has_target_value = true;
					target_value = a2i(target);
				} else {
					has_target_value = false;
				}
				if (!has_source_a_value && isNumber(source_a)) {
					has_source_a_value = true;
					source_a_value = a2i(source_a);
				} else {
					has_target_value = false;
				}
				if (!has_source_b_value && isNumber(source_b)) {
					has_source_b_value = true;
					source_b_value = a2i(source_b);
				} else {
					has_target_value = false;
				}
				if (!showas.empty() && isGlobal())
					showas = FORMAT("__G{}", showas);
			}
			
			bool targetIsSave() {
				switch (type) {
				case LOAD_GLO:
				case LOAD_STA:
				case PLUS:
				case MINUS:
				case MULT:
				case DIV:
				case MOVE:
				case LI:
				case LOAD_ARR_GLO:
				case LOAD_ARR_STA:
				case LA:

				case SCAN_INT:
				case SCAN_CHAR:
					
					return true;
				default:
					return false;
				}
			}

			bool targetIsLoad() {
				switch (type) {
				case PRINT_CHAR:
				case PRINT_INT:
				case SAVE_GLO:
				case SAVE_STA:
				case PUSH_REG:
				case BGEZ:
				case BLEZ:
				case BGTZ:
				case BLT:
				case BGT:
				case BLE:
				case BGE:
				case BEQ:
				case BNE:
				case SAVE_ARR_GLO:
				case SAVE_ARR_STA:
					return true;
				default:
					return false;
				}
			}


			bool sourceAIsSave() {
				switch (type) {
				default:
					return false;
				}
			}



			bool sourceAIsLoad() {
				switch (type) {
				case PLUS:
				case MINUS:
				case MULT:
				case DIV:
				case MOVE:
				case BLT:
				case BGT:
				case BLE:
				case BGE:
				case BEQ:
				case BNE:
					return true;
				default:
					return false;
				}
			}

			bool sourceBIsSave() {
				switch (type) {
				default:
					return false;
				}
			}

			bool sourceBIsLoad() {
				switch (type) {
				case PLUS:
				case MINUS:
				case MULT:
				case DIV:
				case SAVE_ARR_GLO:
				case SAVE_ARR_STA:
				case LOAD_ARR_GLO:
				case LOAD_ARR_STA:
					return true;

				default:
					return false;
				}
			}

			bool isLoad() {
				return targetIsLoad() || sourceAIsLoad() || sourceBIsLoad();
			}

			bool isSave() {
				return targetIsSave() || sourceAIsSave() || sourceBIsSave();
			}

			

			string getStoreName() {
				switch (type) {
				case PLUS:
				case MINUS: 
				case MULT: 
				case DIV: 
				case LA: 
				case MOVE: 
				case LI: 
					return target;
					break;
				case LOAD_GLO:
				case LOAD_STA:
				case LOAD_ARR_GLO:
				case LOAD_ARR_STA:
					return target;
				case SAVE_ARR_GLO:
				case SAVE_ARR_STA:
				case SAVE_GLO:
				case SAVE_STA:
					return showas;

				case SCAN_CHAR:
				case SCAN_GLOBAL_CHAR:
				case SCAN_GLOBAL_INT:
				case SCAN_INT:
					return showas;
				default:;
					// panic("this is not store instr");
				}
				return "";
			}

			vector<string> getLoadName() {
				vector<string> loads;

				switch (type) {
				case LOAD_STA:
				case LOAD_GLO:
					loads.push_back(showas);
					return loads;
				case LOAD_ARR_GLO:
				case LOAD_ARR_STA:
					loads.push_back(source_b);
					loads.push_back(showas);
					return loads;
				case SAVE_STA:
				case SAVE_GLO:
					loads.push_back(target);
					return loads;
				case SAVE_ARR_GLO:
				case SAVE_ARR_STA:
					loads.push_back(target);
					loads.push_back(source_b);
					return loads;
					
				}
				if (isInput())
					return loads;
				if (targetIsLoad() && !has_target_value)
					loads.push_back(target);
				if (sourceAIsLoad() && !has_source_a_value)
					loads.push_back(source_a);
				if (sourceBIsLoad() && !has_source_b_value)
					loads.push_back(source_b);
				return loads;
			}

			// TODO: remove this
			bool doNotConstProp() {
				return isInput() || isArrMemory() || isGlobal();
			}

			// TODO: remove this
			bool doNotDelDead() {
				return isInput() || isArrMemory() || isGlobal();
			}
			//
			// bool isGlobalMem() {
			// 	switch (type) {
			// 	case SAVE_ARR_GLO:
			// 	case SAVE_ARR_STA:
			// 		return true;
			// 	}
			// }

			bool isALU() {
				switch (type) {
				case PLUS:
				case MINUS:
				case MULT:
				case DIV:
					return true;
				default:
					return false;
				}
			}
			
			bool isInput() {
				switch (type) {
				// case PRINT_GLOBAL_STR: 
				// case PRINT_INT: 
				// case PRINT_CHAR:
				case SCAN_INT: 
				case SCAN_CHAR:
				case SCAN_GLOBAL_INT: 
				case SCAN_GLOBAL_CHAR:
				// case PRINT_LINE:
				// case LA:
					return true;
				default:
					return false;
				}
			}

			bool isMemorySave() {
				switch (type) {

				// case LOAD_GLO: 
				// case LOAD_STA: 
				case SAVE_GLO: 
				case SAVE_STA: 
				case SAVE_ARR_GLO: 
				// case LOAD_ARR_GLO: 
				case SAVE_ARR_STA:
					
				// case LOAD_ARR_STA: 
					return true;
				default:
					return false;
				}
			}

			bool isArrMemory() {
				switch (type) {

					// case LOAD_GLO: 
					// case LOAD_STA: 
					// case SAVE_GLO: 
					// case SAVE_STA: 
				case SAVE_ARR_GLO:
				case LOAD_ARR_GLO: 
				case SAVE_ARR_STA:
				case LOAD_ARR_STA: 
					return true;
				default:
					return false;
				}
			}
			
			bool isGlobal() {
				switch (type) {
				case PRINT_GLOBAL_STR: 
				case SCAN_GLOBAL_INT: 
				case SCAN_GLOBAL_CHAR: 
				case LOAD_GLO:
				case SAVE_GLO: 
				case LOAD_ARR_GLO: 
				case SAVE_ARR_GLO: 
					return true;
				default:
					return false;
				}
			}

			void setValue(string reg, int value) {
				if (targetIsLoad() && target == reg) {
					target_value = value;
					has_target_value = true;
					target = i2a(value);
				}
				if (sourceAIsLoad() && source_a == reg) {
					source_a_value = value;
					has_source_a_value = true;
					source_a = i2a(value);
				}
				if (sourceBIsLoad() && source_b == reg) {
					source_b_value = value;
					has_source_b_value = true;
					source_b = i2a(value);
				}
				if (type == LOAD_GLO || type == LOAD_STA) {
					type = LI;
					has_source_a_value = true;
					source_a_value = value;
					source_a = i2a(value);
				}
				if (type == SAVE_GLO || type == SAVE_STA) {
					has_ans = true;
					ans = value;
				}
				// panic("error reg");
			}

			bool isConst() {
				if (isInput())
					return false;
				if (isArrMemory())
					return false;
				if (targetIsLoad() && !has_target_value)
					return false;
				if (sourceAIsLoad() && !has_source_a_value)
					return false;
				if (sourceBIsLoad() && !has_source_b_value)
					return false;
				return true;
			}

			void eval() {
				assert(isConst());
				switch (type) {
				case PLUS:
					ans = source_a_value + source_b_value;
					has_ans = true;
					break;
				case MINUS:
					ans = source_a_value - source_b_value;
					has_ans = true;
					break;
				case MULT:
					ans = source_a_value * source_b_value;
					has_ans = true;
					break;
				case DIV:
					ans = source_a_value / source_b_value;
					has_ans = true;
					break;
				case MOVE:
					ans = source_a_value;
					has_ans = true;
					break;
				case LI:
					ans = a2i(source_a);
					has_ans = true;
					break;
				case BGEZ:
					ans = target_value >= 0 ? 1 : 0;
					break;
				case BLEZ:
					ans = target_value <= 0 ? 1 : 0;
					break;
				case BGTZ:
					ans = target_value > 0 ? 1 : 0;
					break;
				case BLT:
					ans = target_value < source_a_value ? 1 : 0;
					break;
				case BGT:
					ans = target_value > source_a_value ? 1 : 0;
					break;
				case BLE:
					ans = target_value <= source_a_value ? 1 : 0;
					break;
				case BGE:
					ans = target_value >= source_a_value ? 1 : 0;
					break;
				case BEQ:
					ans = target_value == source_a_value ? 1 : 0;
					break;
				case BNE:
					ans = target_value != source_a_value ? 1 : 0;
					break;
				case PRINT_INT:
				case PRINT_CHAR:
					ans = target_value;
					break;
				default:;
					// panic("this type can not eval");
				}
			}

			void optimize() {
				assert(isConst());
				switch (type) {
				case PLUS:
				case MINUS:
				case MULT:
				case DIV:
				case MOVE:
				case LI:
					type = LI;
					source_a = i2a(ans);
					break;
				case BGEZ:
				case BLEZ:
				case BGTZ:
					if (ans == 1) {
						type = JUMP;
						target = source_a;
					}
					else {
						type = NOP;
					}
					break;
				case BLT:
				case BGT:
				case BLE:
				case BGE:
				case BEQ:
				case BNE:
					if (ans == 1) {
						type = JUMP;
						target = source_b;
					} else {
						type = NOP;
					}
					break;
				case PRINT_INT:
				case PRINT_CHAR:
					target = i2a(ans);
					break;
				
				default:;
					// panic("this type can not be optimized");
				}
			}

			// void optimizeLoad() {
				// switch (type) {
				// 	
				// }
				// case LOAD_ARR_GLO:
				// case LOAD_ARR_STA:
				// case SAVE_ARR_GLO:
				// case SAVE_ARR_STA:
					
			// }

			int getAns() {
				assert(has_ans);
				return ans;
			}

			bool isJump() {
				switch (type) {
				case CALL: 
				case RETURN_END:
				case JUMP: 
				case BGEZ: 
				case BLEZ: 
				case BGTZ: 
				case BLT: 
				case BGT:
				case BLE: 
				case BGE: 
				case BEQ:
				case BNE: 
					return true;
				default:
					return false;
				}
			}

			string getJumpTarget() {
				switch (type) {
				case CALL:
				case RETURN_END:
				case JUMP:
					return target;
				case BGEZ:
				case BLEZ:
				case BGTZ:
					return source_a;
				case BLT:
				case BGT:
				case BLE:
				case BGE:
				case BEQ:
				case BNE:
					return source_b;
				default:
					panic("not jump instr");
				}
			}

			void changeJumpTarget(string new_target) {
				switch (type) {
				case CALL:
				case RETURN_END:
				case JUMP:
					target = new_target;
					break;
				case BGEZ:
				case BLEZ:
				case BGTZ:
					source_a = new_target;
					break;
				case BLT:
				case BGT:
				case BLE:
				case BGE:
				case BEQ:
				case BNE:
					source_b = new_target;
					break;
				default:
					panic("not jump instr");
				}
			}
		
		};
			
			
		

	}
		
		
		
}