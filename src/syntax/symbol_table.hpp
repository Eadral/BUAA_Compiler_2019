#ifndef BUAAC_SYMBOL_TABLE_HPP
#define BUAAC_SYMBOL_TABLE_HPP

#include "../meow/core.hpp"
#include "symbol.hpp"
#include "../lex/token.hpp"
#include "symbol_table_result.hpp"

#include <vector>

namespace buaac {
	namespace syntax {
		
		class SymbolTable {

			std::vector<Symbol> _symbol_stack;
			std::vector<Symbol> _unactive_symbol;

			std::vector<int> _scope_index;
			
		public:
			SymbolTable() {
				
			}
			

			SymbolTableResult push(Symbol symbol) {
				_symbol_stack.push_back(symbol);
				return SymbolTableResult::Ok();
			}

			SymbolTableResult pushScope() {
				// debugln("push");
				_scope_index.push_back(_symbol_stack.size());
				return SymbolTableResult::Ok();
			}

			SymbolTableResult popScope() {
				// debugln("pop");
				while (_symbol_stack.size() > _scope_index.back()) {
					_unactive_symbol.push_back(_symbol_stack.back());
					_symbol_stack.pop_back();
				}
				_scope_index.pop_back();
				return SymbolTableResult::Ok();
			}
			
		};
		
	}
}


#endif