#pragma once

#include "../meow/core.hpp"
#include "instr.hpp"

namespace buaac {
	using instr::Instr;
	

	class Block {
	public:

		std::string label;
		std::vector<Instr> instrs;
		
		Block(std::string label): label(label) {
			instrs.clear();
		}
		
		void addInstr(Instr instr) {
			instrs.emplace_back(instr);
		}

		
	};

	using Blocks = std::vector<Block>;
	
	using BlocksPtr = std::shared_ptr<Blocks>;
	
}

