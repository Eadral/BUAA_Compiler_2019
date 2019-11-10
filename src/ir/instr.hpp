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
				SCAN_INT,
				SCAN_GLOBAL_INT,

				// Empty
				PRINT_LINE,

				// ALU
				PLUS,
				MINUS,
				MULT,
				DIV,

				// Stack
				PUSH,
				POP,
				
				// Memory
				LOAD_LAB,
				LOAD_STA,
				SAVE_LAB,
				SAVE_STA,
				
			} type;

			std::string target, source_a, source_b;
			
			Instr(Type type) : type(type) {}
			Instr(Type type, std::string target) : type(type), target(target) {}
			Instr(Type type, std::string target, std::string source_a) : type(type), target(target), source_a(source_a) {}
			Instr(Type type, std::string target, std::string source_a, std::string source_b)
				: type(type), target(target), source_a(source_a), source_b(source_b) {}
		};

	}
		
		
		
}