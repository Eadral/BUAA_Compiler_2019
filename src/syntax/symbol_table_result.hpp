#ifndef SYMBOL_TABLE_RESULT_HPP
#define SYMBOL_TABLE_RESULT_HPP

#include "../meow/core.hpp"
#include "symbol.hpp"

ENUM_START(SymbolTableError)
REDEFINE,
UNDEFINED,
FUNC_PARAM_NUM_UNMATCH,
FUNC_PARAM_TYPE_UNMATCH,
ENUM_MED(SymbolTableError)
ENUM_DEFAULT_OUTPUT(REDEFINE)
ENUM_DEFAULT_OUTPUT(UNDEFINED)
ENUM_END()

using SymbolTableResult = Error<SymbolTableError>;

#endif