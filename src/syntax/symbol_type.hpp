#ifndef SYMBOL_TYPE_HPP
#define SYMBOL_TYPE_HPP

#include "../meow/core.hpp"

ENUM_START(SymbolType)
INT,
CHAR,
FUNC_INT,
FUNC_CHAR,
FUNC_VOID,
ENUM_MED(SymbolType)

ENUM_FUNC(name)

bool isFunc() {
	return type_ == FUNC_INT || type_ == FUNC_CHAR || type_ == FUNC_VOID;
}

bool isVar() {
	return type_ == INT || type_ == CHAR;
}

ENUM_FUNC_END(name)

#endif