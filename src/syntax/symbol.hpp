#ifndef BUAAC_SYMBOL_HPP
#define BUAAC_SYMBOL_HPP

#include "../meow/core.hpp"
#include "../lex/token.hpp"
#include "symbol_type.hpp"


namespace buaac {

	using buaac::lex::Token;
	
	namespace syntax {

		class Symbol {

			std::string _ident;
			SymbolType _symbol_type;
			
			bool _is_const;
			std::string _const_value{};
			
			bool _is_array;
			int _array_len = 0;
			
			std::vector<SymbolType> _function_parameters{};
			
		public:
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


			Symbol(const Symbol& other)
				: _ident(other._ident),
				  _symbol_type(other._symbol_type),
				  _is_const(other._is_const),
				  _const_value(other._const_value),
				  _is_array(other._is_array),
				  _array_len(other._array_len),
				  _function_parameters(other._function_parameters) {
			}

			Symbol(Symbol&& other) noexcept
				: _ident(std::move(other._ident)),
				  _symbol_type(std::move(other._symbol_type)),
				  _is_const(other._is_const),
				  _const_value(std::move(other._const_value)),
				  _is_array(other._is_array),
				  _array_len(other._array_len),
				  _function_parameters(std::move(other._function_parameters)) {
			}

			Symbol& operator=(const Symbol& other) {
				if (this == &other)
					return *this;
				_ident = other._ident;
				_symbol_type = other._symbol_type;
				_is_const = other._is_const;
				_const_value = other._const_value;
				_is_array = other._is_array;
				_array_len = other._array_len;
				_function_parameters = other._function_parameters;
				return *this;
			}

			Symbol& operator=(Symbol&& other) noexcept {
				if (this == &other)
					return *this;
				_ident = std::move(other._ident);
				_symbol_type = std::move(other._symbol_type);
				_is_const = other._is_const;
				_const_value = std::move(other._const_value);
				_is_array = other._is_array;
				_array_len = other._array_len;
				_function_parameters = std::move(other._function_parameters);
				return *this;
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
		};

		
		
	}
}

#endif
