#ifndef BUAAC_SYMBOL_TABLE_HPP
#define BUAAC_SYMBOL_TABLE_HPP

#include "../meow/core.hpp"
#include "symbol.hpp"
#include "../lex/token.hpp"
// #include "symbol_table_result.hpp"

#include <vector>

namespace buaac {
	namespace syntax {
		
		class SymbolTable {

			std::vector<Symbol> _symbol_stack;
			std::vector<Symbol> _unactive_symbol;

			std::vector<int> _scope_index{-1};
			
		public:
			SymbolTable() {
				
			}
			

			bool push(Symbol symbol) {
				if (identExistInScope(symbol.getIdent())) {
					return false;
				}
				if (_scope_index.back() == -1) {
					symbol._is_global = true;
				} else {
					symbol._is_global = false;
				}
				_symbol_stack.push_back(symbol);
				return true;
			}

			void pushScope() {
				// debugln("exprPush");
				_scope_index.push_back(_symbol_stack.size());
				// return SymbolTableResult::Ok();
			}

			void popScope() {
				// debugln("pop");
				while (_symbol_stack.size() > _scope_index.back()) {
					_unactive_symbol.push_back(_symbol_stack.back());
					_symbol_stack.pop_back();
				}
				_scope_index.pop_back();
				// return SymbolTableResult::Ok();
			}

			bool isGlobal(std::string ident) {
				Symbol symbol;
				tie(std::ignore, symbol) = findSymbol(ident);
				return symbol._is_global;
			}

			bool isConst(std::string ident) {
				Symbol symbol;
				tie(std::ignore, symbol) = findSymbol(ident);
				return symbol.isConst();
			}

			bool isChar(std::string ident) {
				Symbol symbol;
				tie(std::ignore, symbol) = findSymbol(ident);
				return symbol.getType().type_ == SymbolType::CHAR;
			}


			std::string getConstValue(std::string ident) {
				Symbol symbol;
				tie(std::ignore, symbol) = findSymbol(ident);
				return symbol.getConstValue();
			}
			
			tuple<bool, Symbol> findSymbol(std::string ident) {
				for (int i = _symbol_stack.size() - 1; i >= 0; i--) {
					if (_symbol_stack[i].getIdent() == ident) {
						return make_tuple(true, _symbol_stack[i]);
					}
				}
				return make_tuple(false, Symbol{});
			}

			tuple<int, Symbol> findSymbolAndScope(std::string ident) {
				for (int i = _symbol_stack.size() - 1; i >= 0; i--) {
					if (_symbol_stack[i].getIdent() == ident) {
						return make_tuple(i, _symbol_stack[i]);
					}
				}
				return make_tuple(-1, Symbol{});
			}

			// bool identExist(std::string ident) {
			// 	bool is_exist;
			// 	tie(is_exist, std::ignore) = findSymbol(ident);
			// 	return is_exist;
			// }

			tuple<bool, Symbol> findSymbolInScope(std::string ident) {
				for (int i = _symbol_stack.size() - 1; i >= 0 && i >= _scope_index.back(); i--) {
					if (_symbol_stack[i].getIdent() == ident) {
						return make_tuple(true, _symbol_stack[i]);
					}
				}
				return make_tuple(false, Symbol{});
			}
			
			bool identExistInScope(std::string ident) {
				bool is_exist;
				tie(is_exist, std::ignore) = findSymbolInScope(ident);
				return is_exist;
			}

			// bool isType(std::string ident, SymbolType symbol_type) {
			// 	bool is_exist;
			// 	Symbol symbol;
			// 	tie(is_exist, symbol) = findSymbol(ident);
			// 	assert(is_exist);
			// 	return symbol.getType() == symbol_type;
			// }
			//
			// bool isConst(std::string ident) {
			// 	bool is_exist;
			// 	Symbol symbol;
			// 	tie(is_exist, symbol) = findSymbol(ident);
			// 	assert(is_exist);
			// 	return symbol.isConst();
			// }

			int getScope() {
				return _scope_index.back();
			}

			int getStackBytesByIdent(std::string ident) {
				int s = 0;
				for (int i = _scope_index.back(); i < _symbol_stack.size(); i++) {
					if (_symbol_stack[i].getIdent() == ident)
						break;
					s += getSymbolBytes(_symbol_stack[i]);
				}
				return -s;
			}
			
			int getStackBytesById(int id) {
				int s = 0;
				for (int i = _scope_index.back(); i < id && i < _symbol_stack.size(); i++) {
					s += getSymbolBytes(_symbol_stack[i]);
				}
				return s;
			}
			
			int getStackScopeBytes() {
				int s = 0;
				for (int i = _scope_index.back(); i < _symbol_stack.size(); i++) {
					s += getSymbolBytes(_symbol_stack[i]);
				}
				return s;
			}

			int getGlobalBytesByIdent(std::string ident) {
				int s = 0;
				for (int i = 0; i < _scope_index[1]; i++) {
					if (_symbol_stack[i].getIdent() == ident)
						break;
					s += getSymbolBytes(_symbol_stack[i]);
				}
				return s;
			}
			
			int getGlobalScopeBytes() {
				int s = 0;
				for (int i = 0; i < _scope_index[1]; i++) {
					s += getSymbolBytes(_symbol_stack[i]);
				}
			}

			int getSymbolBytes(Symbol symbol) {
				if (symbol.isArray())
					return 4 * symbol.getArrayLen();
				else
					return 4;
			}
			
		};
		
	}
}


#endif