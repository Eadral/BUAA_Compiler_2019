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
				SAVE_STA_ARR_GLO,
				LOAD_STA_ARR_GLO,
				SAVE_STA_ARR_STA,
				LOAD_STA_ARR_STA,

				// JUMP
				CALL,
				RETURN_END,
				JUMP,
				BGEZ,
				BLEZ,
				BLT,
				BGT,
				BLE,
				BGE,
				BEQ,
				BNE,

				// IR
				IR_SHOW,
				
			} type;

			std::string target{}, source_a{}, source_b{};
			bool show = true;
			std::string showas;
			
			Instr(Type type) : type(type) {}
			Instr(Type type, std::string target)
				: type(type), target(target) {}
			Instr(Type type, std::string target, std::string source_a)
				: type(type), target(target), source_a(source_a) {}
			Instr(Type type, std::string target, std::string source_a, std::string source_b)
				: type(type), target(target), source_a(source_a), source_b(source_b) {}

			Instr(Type type, int target)
				: type(type), target(i2a(target)) {}
			Instr(Type type, int target, std::string source_a)
				: type(type), target(i2a(target)), source_a(source_a) {}
			Instr(Type type, std::string target, int source_a)
				: type(type), target(target), source_a(i2a(source_a)) {}
			Instr(Type type, int target, std::string source_a, std::string source_b)
				: type(type), target(i2a(target)), source_a(source_a), source_b(source_b) {}
			Instr(Type type, std::string target, int source_a, std::string source_b)
				: type(type), target(target), source_a(i2a(source_a)), source_b(source_b) {}
			Instr(Type type, std::string target, std::string source_a, int source_b)
				: type(type), target(target), source_a(source_a), source_b(i2a(source_b)) {}

			// Instr(const Instr& other)
			// 	: type(other.type),
			// 	  target(other.target),
			// 	  source_a(other.source_a),
			// 	  source_b(other.source_b) {
			// }
			//
			// Instr(Instr&& other) noexcept
			// 	: type(other.type),
			// 	  target(std::move(other.target)),
			// 	  source_a(std::move(other.source_a)),
			// 	  source_b(std::move(other.source_b)) {
			// }
			//
			// Instr& operator=(const Instr& other) {
			// 	if (this == &other)
			// 		return *this;
			// 	type = other.type;
			// 	target = other.target;
			// 	source_a = other.source_a;
			// 	source_b = other.source_b;
			// 	return *this;
			// }
			//
			// Instr& operator=(Instr&& other) noexcept {
			// 	if (this == &other)
			// 		return *this;
			// 	type = other.type;
			// 	target = std::move(other.target);
			// 	source_a = std::move(other.source_a);
			// 	source_b = std::move(other.source_b);
			// 	return *this;
			// }
		};

	}
		
		
		
}