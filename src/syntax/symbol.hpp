#ifndef BUAAC_SYMBOL_HPP
#define BUAAC_SYMBOL_HPP

#include "../meow/core.hpp"
#include "../lex/token.hpp"
#include "../meow/fmt/display.hpp"
#include "symbol_type.hpp"



namespace buaac {

	using buaac::lex::Token;
	
	namespace syntax {

		class Symbol : implement fmt::Display {

			std::string _ident;
			SymbolType _symbol_type;
			
			bool _is_const;
			std::string _const_value{};
			
			bool _is_array;
			int _array_len = 0;
			
			std::vector<SymbolType> _function_parameters{};
			
		public:

			bool _is_global{0};
			
			Symbol() {
				
			}

			Symbol(SymbolType symbol_type, std::string ident, std::string const_value = "")
				: _ident(ident), _symbol_type(symbol_type)
			{
				assert(symbol_type.isVar());
				if (const_value == "") {
					_is_const = false;
				} else {
					_is_const = true;
					_const_value = const_value;
				}
				_is_array = false;
				_array_len = 0;
				
			}

			Symbol(SymbolType symbol_type, std::string ident, int array_len)
				: _ident(ident), _symbol_type(symbol_type)
			{
				assert(symbol_type.isVar());
				_is_const = false;
				_is_array = true;
				_array_len = array_len;
				
			}

			Symbol(SymbolType symbol_type, std::string ident, std::vector<SymbolType> function_parameters)
				: _ident(ident), _symbol_type(symbol_type), _function_parameters(function_parameters)
			{
				assert(symbol_type.isFunc());
				_is_const = false;
				_is_array = false;

			}

			friend bool operator==(const Symbol& lhs, const Symbol& rhs) {
				return lhs._ident == rhs._ident
					&& lhs._symbol_type == rhs._symbol_type
					&& lhs._is_const == rhs._is_const
					&& lhs._const_value == rhs._const_value
					&& lhs._is_array == rhs._is_array
					&& lhs._array_len == rhs._array_len
					&& lhs._function_parameters == rhs._function_parameters;
			}

			friend bool operator!=(const Symbol& lhs, const Symbol& rhs) {
				return !(lhs == rhs);
			}

			std::string getIdent() const {
				return _ident;
			}

			SymbolType getType() const {
				return _symbol_type;
			}

			bool isConst() const {
				return _is_const;
			}

			std::string getConstValue() {
				return _const_value;
			}

			bool isArray() const {
				return _is_array;
			}

			int getArrayLen() const {
				return _array_len;
			}

			std::vector<SymbolType> getParaList() const {
				return _function_parameters;
			}

			std::string toString() {
				return _ident;
			}
		};

		
		
	}
}

#endif
