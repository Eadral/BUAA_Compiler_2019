#pragma once

#include "../meow/core.hpp"
#include "ir.hpp"

#include <map>

namespace buaac {

	class Interpreter {

		// std::map<string, int> temp;
		// std::map<string, int> local_var;
		// std::map<string, int> global_var;
		std::map<string, int> var;


		
	public:
		Interpreter() {
			
		}

		void eval(Instr &instr) {
			setInstrValue(instr);
		}

		void setInstrValue(Instr &instr) {
			if (instr.type == Instr::IR_SHOW)
				return;
			
			const auto &names = instr.getLoadName();
			for (int i = 0; i < names.size(); i++) {
				const auto& name = names[i];
				if (hasVar(name)) {
					instr.setValue(name, getVar(name));
				}
			}
			if (instr.doNotConstProp())
				return;
			if (instr.isConst()) {
				instr.eval();
				instr.optimize();
			}
			
			auto store_name = instr.getStoreName();
			int ans;
			if (instr.has_ans) {
				ans = instr.getAns();
				setVar(store_name, ans);
			} 
			// }
		}

		void setVar(string name, int value) {
			var[name] = value;
		}

		bool hasVar(string name) {
			return var.find(name) != var.end();
		}
		
		int getVar(string name) {
			assert(hasVar(name));
			return var[name];
		}

		// void setLocalValue(string name, int value) {
		// 	local_var[name] = value;
		// }
		//
		// void setGlobalValue(string name, int value) {
		// 	global_var[name] = value;
		// }
		//
		// bool hasLocalValue(string name) {
		// 	return local_var.find(name) != local_var.end();
		// }
		//
		// bool hasGlobalValue(string name) {
		// 	return global_var.find(name) != global_var.end();
		// }
		//
		// int getLocalValue(string name) {
		// 	assert(hasLocalValue(name));
		// 	return local_var[name];
		// }

		
	};
	
}
