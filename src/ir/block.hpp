#pragma once

#include "../meow/core.hpp"
#include "instr.hpp"

namespace buaac {
	using instr::Instr;
	

	class Block {
	public:

		string label;
		vector<Instr> instrs;
		
		Block(string label): label(label) {
			instrs.clear();
		}
		
		void addInstr(Instr instr) {
			instrs.emplace_back(instr);
		}

		
	};

	using Blocks = vector<Block>;
	
	using BlocksPtr = std::shared_ptr<Blocks>;
	
}

