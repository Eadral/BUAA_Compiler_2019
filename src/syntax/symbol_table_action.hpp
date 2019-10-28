#ifndef SYMBOL_TABLE_ACTION_HPP
#define SYMBOL_TABLE_ACTION_HPP

#include "../meow/core.hpp"


ENUM_START(Action)
INSCOPE,
OUTSCOPE,
ENUM_MED(Action)
ENUM_DEFAULT_OUTPUT(INSCOPE)
ENUM_DEFAULT_OUTPUT(OUTSCOPE)
ENUM_END()

class SymbolOrAction {

	Symbol _symbol;
	Action _action;
	bool _is_symbol;
	bool _is_action;

public:
	SymbolOrAction() {
		_is_symbol = false;
		_is_action = false;
	}
	SymbolOrAction(Symbol symbol) : _symbol(symbol), _is_symbol(true), _is_action(false) {}
	SymbolOrAction(Action action) : _action(action), _is_symbol(false), _is_action(true) {}
	SymbolOrAction(Action::Type action_type) : _action(Action(action_type)), _is_symbol(false), _is_action(true) {}

	bool isSymbol() {
		return _is_symbol;
	}

	bool isAction() {
		return _is_action;
	}

	Symbol getSymbol() {
		return _symbol;
	}

	Action getAction() {
		return _action;
	}

	SymbolOrAction(const SymbolOrAction& other)
		: _symbol(other._symbol),
		_action(other._action),
		_is_symbol(other._is_symbol),
		_is_action(other._is_action) {
	}

	SymbolOrAction(SymbolOrAction&& other) noexcept
		: _symbol(std::move(other._symbol)),
		_action(std::move(other._action)),
		_is_symbol(other._is_symbol),
		_is_action(other._is_action) {
	}

	SymbolOrAction& operator=(const SymbolOrAction& other) {
		if (this == &other)
			return *this;
		_symbol = other._symbol;
		_action = other._action;
		_is_symbol = other._is_symbol;
		_is_action = other._is_action;
		return *this;
	}

	SymbolOrAction& operator=(SymbolOrAction&& other) noexcept {
		if (this == &other)
			return *this;
		_symbol = std::move(other._symbol);
		_action = std::move(other._action);
		_is_symbol = other._is_symbol;
		_is_action = other._is_action;
		return *this;
	}


};

#endif