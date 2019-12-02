#ifndef BUAAC_SYNTAX_PARSE_HPP
#define BUAAC_SYNTAX_PARSE_HPP

#include "../meow/core.hpp"
#include "../lex/lexparse.hpp"
#include "symbol_table.hpp"
#include "../output_helper.hpp"
#include "../ir/ir.hpp"

#include <map>
#include <cstdlib>

#define ERROR ;	// ignore all error

namespace buaac {
namespace syntax{

	using std::tuple;
	using std::make_tuple;
	using std::tie;


	
	class SyntaxParser {

		using Token = lex::Token;
		using TokenType = lex::TokenType;
		
		Output* _output = Output::getInstance();
		SymbolTable _symbol_table;

		IR ir;
		
	public:
		SyntaxParser(lex::LexParser& lex_parser, std::string output): lex_parser_(lex_parser) {
			if (output.find('v') != std::string::npos) {
				output_syntax_ = true;
			}
			if (output.find('e') != std::string::npos) {
				output_error_ = true;
			}
		}

		IR start() {
			program();
			return ir;
		}

		int getLineNumber() const {
			return lex_parser_.getLineNumber();
		}

	private:

		bool _push_success;
#define checkPush(push_action)	\
	do {	\
	_push_success = push_action;	\
		if (!_push_success) {	\
			error('b');	\
		}	\
	} while (0)	

		bool _find_success;
#define checkFind()	\
	do {	\
		if (!_find_success) {	\
			error('c');	\
		}	\
	} while (0)
		
		std::map<std::string, bool> funcName2IsRet;

		bool stack_output_constDec_ = false;
		bool stack_output_verDec_ = false;
		bool stack_output_statementCol_ = false;

		bool output_syntax_ = false;
		bool output_error_ = false;

		void resetOuput() {
			stack_output_constDec_ = false;
			stack_output_verDec_ = false;
			stack_output_statementCol_ = false;
		}
		
		void syntaxOutput(std::string output) {
			if (!output_syntax_)
				return;
			if (output == "<常量说明>") {
				if (!stack_output_constDec_) {
					std::cout << output << std::endl;
					stack_output_constDec_ = true;
				}
				stack_output_verDec_ = false;
				stack_output_statementCol_ = false;
			} else if (output == "<变量说明>") {
				if (!stack_output_verDec_) {
					std::cout << output << std::endl;
					stack_output_verDec_ = true;
				}
				stack_output_constDec_ = false;
				stack_output_statementCol_ = false;
			} else if (output == "<语句列>") {
				if (!stack_output_statementCol_) {
					std::cout << output << std::endl;
					stack_output_statementCol_ = true;
				}
				stack_output_constDec_ = false;
				stack_output_verDec_ = false;
			} else {
				std::cout << output << std::endl;
			}
		} 
		
#pragma region GeneratedSyntax
		// ＜加法运算符＞ ::= +｜-
		Token plusOp() {
			Token op_token;
			
			switch (lookTokenType()) {
			case TokenType::PLUS:
				op_token = eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				op_token = eatToken(TokenType::MINU);
				break;
			default:
				ERROR
			}
			return op_token;
		}
		// ＜乘法运算符＞  ::= *｜/
		void mulOp() {
			switch (lookTokenType()) {
			case TokenType::MULT:
				eatToken(TokenType::MULT);
				ir.exprPush(IR::MULT);
				break;
			case TokenType::DIV:
				eatToken(TokenType::DIV);
				ir.exprPush(IR::DIV);
				break;
			default:
				ERROR
			}
		}
		// ＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
		Token relationOp() {
			Token cmp_token;
			switch (lookTokenType()) {
			case TokenType::LSS:
				cmp_token = eatToken(TokenType::LSS);
				break;
			case TokenType::LEQ:
				cmp_token = eatToken(TokenType::LEQ);
				break;
			case TokenType::GEQ:
				cmp_token = eatToken(TokenType::GEQ);
				break;
			case TokenType::GRE:
				cmp_token = eatToken(TokenType::GRE);
				break;
			case TokenType::EQL:
				cmp_token = eatToken(TokenType::EQL);
				break;
			case TokenType::NEQ:
				cmp_token = eatToken(TokenType::NEQ);
				break;
			default:
				ERROR
			}
			return cmp_token;
		}
		// ＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
		std::string strconst() {
			Token str_token;
			
			switch (lookTokenType()) {
			case TokenType::STRCON:
				str_token = eatToken(TokenType::STRCON);
				break;
			default:
				ERROR
			}
			syntaxOutput("<字符串>");
			return str_token.getValue();
		}
		// ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
		void program() {
			_para_cnt = 0;
			switch (lookTokenType()) {
			case TokenType::CONSTTK:
			case TokenType::INTTK:
			case TokenType::CHARTK:
			case TokenType::VOIDTK:
				// ［＜常量说明＞］
				constDec();
				resetOuput();
				// ［＜变量说明＞］
				verDec();
				
				resetOuput();
				// {＜有返回值函数定义＞|＜无返回值函数定义＞}
				funcDefGroup();
				// ＜主函数＞
				mainFunc();
				break;
			default:
				ERROR
				program();
				return;
			}
			syntaxOutput("<程序>");
		}
		// {＜有返回值函数定义＞|＜无返回值函数定义＞}
		void funcDefGroup() {
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				funcDef();
				funcDefGroup();
				break;
			case TokenType::VOIDTK:
				switch (lookTokenType(1)) {
				case TokenType::IDENFR:
					funcDef();
					funcDefGroup();
					break;
				case TokenType::MAINTK:
					// Empty
					return;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
		}
		SymbolType _status_func_defining_type;
		int _status_value_ret_cnt;
		// ＜有返回值函数定义＞|＜无返回值函数定义＞
		void funcDef() {
			// ir.pushStackReg("$ra");
			_status_value_ret_cnt = 0;
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				if (lookTokenType() == TokenType::INTTK) {
					_status_func_defining_type = SymbolType::FUNC_INT;
				} else {
					_status_func_defining_type = SymbolType::FUNC_CHAR;
				}
				retFuncDef();
				if (_status_value_ret_cnt == 0) {
					error('h');
				}
				break;
			case TokenType::VOIDTK:
				_status_func_defining_type = SymbolType::FUNC_VOID;
				nonRetFuncDef();
				break;
			default:
				ERROR
			}
			ir.appendInstr(Instr(Instr::RETURN_END)); 
		}
		// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
		void constDec() {
			switch (lookTokenType()) {
			case TokenType::CONSTTK:
				eatToken(TokenType::CONSTTK);
				constDef();
				eatToken(TokenType::SEMICN);
				constDec();
				break;
			// Empty
			case TokenType::IDENFR:
			case TokenType::INTTK:
			case TokenType::CHARTK:
			case TokenType::VOIDTK:
			case TokenType::IFTK:
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
			case TokenType::SCANFTK:
			case TokenType::PRINTFTK:
			case TokenType::RETURNTK:
			case TokenType::SEMICN:
			case TokenType::LBRACE:
			case TokenType::RBRACE:
				return;
				break;
			default:
				ERROR
			}
			syntaxOutput("<常量说明>");
		}
		// ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
		//		| char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }

		void constDef() {
			switch (lookTokenType()) {
			case TokenType::INTTK:
				// int＜标识符＞＝＜整数＞
				constDefInt();
				// {,＜标识符＞＝＜整数＞}
				constDefIntGroup();
				break;
			case TokenType::CHARTK:
				// char＜标识符＞＝＜字符＞
				constDefChar();
				// { ,＜标识符＞＝＜字符＞ }
				constDefCharGroup();
				break;
			default:
				ERROR
			}
			syntaxOutput("<常量定义>");
		}
		// int＜标识符＞＝＜整数＞
		void constDefInt() {
			Token ident_token;
			Token int_token;
			bool int_success;

			std::vector<TokenType> except_end = { TokenType::COMMA, TokenType::SEMICN };
			
			switch (lookTokenType()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				ident_token = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				tie(int_success, int_token) = integer();
				if (!int_success || !exceptTokens(except_end)) {
					error('o');
					jumpUntil(except_end);
				}
				break;
			default:
				ERROR
			}
			
			checkPush(_symbol_table.push(Symbol(SymbolType::INT, ident_token.getValue(), int_token.getValue())));
		}
		// char＜标识符＞＝＜字符＞
		void constDefChar() {
			Token ident_token;
			Token char_token;

			std::vector<TokenType> except_end = { TokenType::COMMA, TokenType::SEMICN };
			
			switch (lookTokenType()) {
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				ident_token = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				if (lookTokenType() != TokenType::CHARCON) {
					error('o');
					jumpUntil(except_end);
					break;
				}
				char_token = eatToken(TokenType::CHARCON);
				if (!exceptTokens(except_end)) {
					error('o');
					jumpUntil(except_end);
				}
				break;
			default:
				ERROR
			}
			checkPush(_symbol_table.push(Symbol(SymbolType::CHAR, ident_token.getValue(), i2a(int(char_token.getValue()[0])))));
		}
		// {,＜标识符＞＝＜整数＞}
		void constDefIntGroup() {
			Token ident_token;
			Token int_token;
			bool int_success;

			std::vector<TokenType> except_end = { TokenType::COMMA, TokenType::SEMICN };
			
			switch (lookTokenType()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				ident_token = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				tie(int_success, int_token) = integer();
				if (!int_success || !exceptTokens(except_end)) {
					error('o');
					jumpUntil(except_end);
				}
				constDefIntGroup();
				break;
				// STOP
			case TokenType::SEMICN:
				return;
			default:
				ERROR
			}
			checkPush(_symbol_table.push(Symbol(SymbolType::INT, ident_token.getValue(), int_token.getValue())));
		}
		// { ,＜标识符＞＝＜字符＞ }
		void constDefCharGroup() {
			Token ident_token;
			Token char_token;

			std::vector<TokenType> except_end = { TokenType::COMMA, TokenType::SEMICN };
			
			switch (lookTokenType()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				ident_token = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				if (lookTokenType() != TokenType::CHARCON) {
					error('o');
					jumpUntil(except_end);
					break;
				}
				char_token = eatToken(TokenType::CHARCON);
				if (!exceptTokens(except_end)) {
					error('o');
					jumpUntil(except_end);
				}
				constDefCharGroup();
				break;
				// STOP
			case TokenType::SEMICN:
				return;
			default:
				ERROR
			}
			checkPush(_symbol_table.push(Symbol(SymbolType::CHAR, ident_token.getValue(), i2a(int(char_token.getValue()[0])))));
		}
		// ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝| 0
		Token uninteger() {
			Token intcon;
			std::string intcon_str;
			switch (lookTokenType()) {
			case TokenType::INTCON:
				intcon = eatToken(TokenType::INTCON);
				intcon_str = intcon.getValue();
				if (intcon_str != "0" && intcon_str[0] == '0') {
					error('a');
				}
				break;
			default:
				ERROR
			}
			syntaxOutput("<无符号整数>");
			return intcon;
		}
		// ＜整数＞        ::= ［＋｜－］＜无符号整数＞
		tuple<bool, Token> integer() {
			Token ret;
			std::string neg;
			switch (lookTokenType()) {
			case TokenType::INTCON:
				ret = uninteger();
				break;
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				ret = uninteger();
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				ret = uninteger();
				neg = FORMAT("-{}", ret.getValue());
				ret = Token(TokenType::INTCON, neg);
				break;
			default:
				return make_tuple(false, Token{});
			}
			syntaxOutput("<整数>");
			return make_tuple(true, ret);
		}
		
		
		// ＜声明头部＞   ::=  int＜标识符＞ | char＜标识符＞
		tuple<Token, Token> decHead() {
			Token type;
			Token idenfr;
			switch (lookTokenType()) {
			case TokenType::INTTK:
				type = eatToken(TokenType::INTTK);
				idenfr = eatToken(TokenType::IDENFR);
				break;
			case TokenType::CHARTK:
				type = eatToken(TokenType::CHARTK);
				idenfr = eatToken(TokenType::IDENFR);
				break;
			default:
				ERROR
			}
			syntaxOutput("<声明头部>");
			return make_tuple(type, idenfr);
		}

		// ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
		void verDec() {
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				switch (lookTokenType(2)) {
				case TokenType::SEMICN:
				case TokenType::COMMA:
				case TokenType::LBRACK:
					// {＜变量定义＞;}
					verDef();
					eatToken(TokenType::SEMICN);
					verDec();
					
					
					break;
				case TokenType::LPARENT:
					return;
				default:
					ERROR
				}
				break;
			// EMPTY
			case TokenType::IDENFR:
			case TokenType::VOIDTK:
			case TokenType::IFTK:
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
			case TokenType::SCANFTK:
			case TokenType::PRINTFTK:
			case TokenType::RETURNTK:
			case TokenType::SEMICN:
			case TokenType::LBRACE:
			case TokenType::RBRACE:
				return;
			default:
				ERROR
			}
			syntaxOutput("<变量说明>");
		}

		SymbolType _status_vardef_type;
		std::string _status_vardef_type_str;
		//＜变量定义＞ ::= ＜类型标识符＞
		//				(＜标识符＞|＜标识符＞'['＜无符号整数＞']')
		//				{,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )} 
		void verDef() {
			Token type;
			Token ident;
			
			Token uninteger_token;
			
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				// ＜类型标识符＞
				type = typeIdent();
				if (type.getTokenType() == TokenType::INTTK) {
					_status_vardef_type = SymbolType::INT;
					_status_vardef_type_str = "int";
				}
				else if (type.getTokenType() == TokenType::CHARTK) {
					_status_vardef_type = SymbolType::CHAR;
					_status_vardef_type_str = "char";
				}
				// (＜标识符＞|＜标识符＞'['＜无符号整数＞']')
				switch (lookTokenType()) {
				case TokenType::IDENFR:
					// ＜标识符＞
					ident = eatToken(TokenType::IDENFR);
					switch (lookTokenType()) {
					case TokenType::LBRACK:
						// '['＜无符号整数＞']'
						eatToken(TokenType::LBRACK);
						uninteger_token = uninteger();
						if (uninteger_token.hasValue() && uninteger_token.getValue() == "0") {
							error('a');
						}
						eatToken(TokenType::RBRACK);
						if (_symbol_table.getScope() == -1) {
							ir.defineGlobalIntArr(ident.getValue(), a2i(uninteger_token.getValue()));
						} else {
							ir.irShow(FORMAT("{} {}[{}];", _status_vardef_type_str, ident.getValue(), uninteger_token.getValue()));
						}
						checkPush(_symbol_table.push(Symbol(_status_vardef_type, ident.getValue(), atoi(uninteger_token.getValue().c_str()))));
						
						break;
					case TokenType::SEMICN:
					case TokenType::COMMA:
						checkPush(_symbol_table.push(Symbol(_status_vardef_type, ident.getValue())));
						if (_symbol_table.getScope() == -1) {
							ir.defineGlobalInt(ident.getValue());
						} else {
							ir.irShow(FORMAT("{} {};", _status_vardef_type_str, ident.getValue()));
						}
						break;
					default:
						ERROR
					}
					break;
				default:
					ERROR
				}
				//	{,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )} 
				verDefNameGroup();
				break;
			default:
				ERROR
			}
			
			syntaxOutput("<变量定义>");
		}

		void pushStackReg(std::string reg) {
			ir.__pushStackReg(reg);
			// _symbol_table.addOffset(4);
		}

		void popStackReg(std::string reg) {
			ir.__popStackReg(reg);
			// _symbol_table.subOffset(4);
		}

		// {,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )} 
		void verDefNameGroup() {
			Token ident;

			Token uninteger_token;
			
			switch (lookTokenType()) {
			case TokenType::COMMA:
				// ,
				eatToken(TokenType::COMMA);
				switch (lookTokenType()) {
				case TokenType::IDENFR:
					// ＜标识符＞
					ident = eatToken(TokenType::IDENFR);
					switch (lookTokenType()) {
					case TokenType::LBRACK:
						// '['＜无符号整数＞']' 
						eatToken(TokenType::LBRACK);
						uninteger_token = uninteger();
						if (uninteger_token.hasValue() && uninteger_token.getValue() == "0") {
							error('a');
						}
						eatToken(TokenType::RBRACK);
						if (_symbol_table.getScope() == -1) {
							ir.defineGlobalIntArr(ident.getValue(), a2i(uninteger_token.getValue()));
						} else
						ir.irShow(FORMAT("{} {}[{}];", _status_vardef_type_str, ident.getValue(), uninteger_token.getValue()));
						checkPush(_symbol_table.push(Symbol(_status_vardef_type, ident.getValue(), atoi(uninteger_token.getValue().c_str()))));

						break;
					case TokenType::SEMICN:
					case TokenType::COMMA:
						checkPush(_symbol_table.push(Symbol(_status_vardef_type, ident.getValue())));
						if (_symbol_table.getScope() == -1) {
							ir.defineGlobalInt(ident.getValue());
						} else
						ir.irShow(FORMAT("{} {};", _status_vardef_type_str, ident.getValue()));
						break;
					default:
						ERROR
					}
					break;
				default:
					ERROR
				}
				// Group
				verDefNameGroup();
				break;
			case TokenType::SEMICN:
				break;
			default:
				ERROR
			}
		}

		// ＜类型标识符＞ ::=  int | char
		Token typeIdent() {
			Token type;
			switch (lookTokenType()) {
			case TokenType::INTTK:
				type = eatToken(TokenType::INTTK);
				break;
			case TokenType::CHARTK:
				type = eatToken(TokenType::CHARTK);
				break;
			default:
				ERROR
			}
			return type;
		}
		// ＜有返回值函数定义＞ ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
		void retFuncDef() {
			Token type;
			Token name;
			SymbolType func_type;
			std::vector<Symbol> para_list;
			
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				//  ＜声明头部＞
				tie(type, name) = decHead();
				ir.defineFunc(name.getValue());
				if (type.getTokenType() == TokenType::INTTK) {
					func_type = SymbolType::FUNC_INT;
				} else if (type.getTokenType() == TokenType::CHARTK) {
					func_type = SymbolType::FUNC_CHAR;
				} else {
					panic("unexcepted function type define");
				}
				funcName2IsRet[name.getValue()] = true;	
				// '('＜参数表＞')' 
				eatToken(TokenType::LPARENT);
				_status_paralist.clear();
				para_list = paraList();
				_para_cnt = para_list.size();
				eatToken(TokenType::RPARENT);
				checkPush(_symbol_table.push(Symbol(func_type, name.getValue(), symbolList2SymbolTypeList(para_list))));
				// '{'＜复合语句＞'}'
				eatToken(TokenType::LBRACE);
				_symbol_table.pushScope();
				for (int i = 0; i < para_list.size(); i++) {
					checkPush(_symbol_table.push(para_list[i]));
				}
				// debugln("exprPush at {}", getLineNumber());
				compStatement();
				eatToken(TokenType::RBRACE);
				ir.popStack(_symbol_table.getStackScopeBytes());
				// _symbol_table.clearOffset();
				_symbol_table.popScope();
				// debugln("pop at {}", getLineNumber());
				break;
			default:
				ERROR
			}
			// // debugln("!{}", name.getValue());
			syntaxOutput("<有返回值函数定义>");
		}
		int _para_cnt = 0;
		// ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
		void nonRetFuncDef() {
			Token name;
			std::vector<Symbol> para_list;
			
			switch (lookTokenType()) {
			case TokenType::VOIDTK:
				// void
				eatToken(TokenType::VOIDTK);
				// ＜标识符＞
				name = eatToken(TokenType::IDENFR);
				ir.defineFunc(name.getValue());
				// '('＜参数表＞')'
				eatToken(TokenType::LPARENT);
				para_list = paraList();
				_para_cnt = para_list.size();
				checkPush(_symbol_table.push(Symbol(SymbolType::FUNC_VOID, name.getValue(), symbolList2SymbolTypeList(para_list))));
				eatToken(TokenType::RPARENT);
				// '{'＜复合语句＞'}'
				eatToken(TokenType::LBRACE);
				_symbol_table.pushScope();
				for (int i = 0; i < para_list.size(); i++) {
					checkPush(_symbol_table.push(para_list[i]));
				}
				// debugln("exprPush at {}", getLineNumber());
				compStatement();
				eatToken(TokenType::RBRACE);
				ir.popStack(_symbol_table.getStackScopeBytes());
				// _symbol_table.clearOffset();
				_symbol_table.popScope();
				// debugln("pop at {}", getLineNumber());
				break;
			default:
				ERROR
			}
			// // debugln("!{}", name.getValue());
			funcName2IsRet[name.getValue()] = false;
			syntaxOutput("<无返回值函数定义>");
		}

		// ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
		void compStatement() {
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::CONSTTK:
			case TokenType::INTTK:
			case TokenType::CHARTK:
			case TokenType::IFTK:
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
			case TokenType::SCANFTK:
			case TokenType::PRINTFTK:
			case TokenType::RETURNTK:
			case TokenType::SEMICN:
			case TokenType::LBRACE:
			case TokenType::RBRACE:
				// ［＜常量说明＞］
				constDec();
				resetOuput();
				// ［＜变量说明＞］
				verDec();
				if (_symbol_table.getScope() == 0) {
					ir.pushStackVars(_symbol_table.getStackScopeBytes());
				}
				else if (_symbol_table.getScope() > 0) {
					ir.pushStackVars(_symbol_table.getStackScopeBytes() - _para_cnt * 4);
				}
				pushStackReg("$ra");
				ir.instrNotShow();
				resetOuput();
				// ＜语句列＞
				statementCol();
				resetOuput();
				ir.newBlock(ir.getReturnLabel());
				popStackReg("$ra");
				ir.instrNotShow();
				break;
			default:
				ERROR
			}
			syntaxOutput("<复合语句>");
		}
		std::vector<Symbol> _status_paralist;
		// ＜参数表＞::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
		std::vector<Symbol> paraList() {
			_status_paralist.clear();
			Token type;
			Token ident;
			
			switch (lookTokenType()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				// ＜类型标识符＞
				type = typeIdent();
				// ＜标识符＞
				ident = eatToken(TokenType::IDENFR);
				_status_paralist.push_back(Symbol(token2SymbolType(type), ident.getValue()));
				ir.irShow(FORMAT("para {} {};", type.getValue(), ident.getValue()));
				// {, ＜类型标识符＞＜标识符＞}
				paraListGroup();
				break;
			case TokenType::RPARENT:
				break;
			default:
				ERROR
			}
			syntaxOutput("<参数表>");
			return _status_paralist;
		}
		//  {, ＜类型标识符＞＜标识符＞}
		void paraListGroup() {
			Token type;
			Token ident;
			
			switch (lookTokenType()) {
			case TokenType::COMMA:
				// ,
				eatToken(TokenType::COMMA);
				// ＜类型标识符＞
				type = typeIdent();
				// ＜标识符＞
				ident = eatToken(TokenType::IDENFR);
				_status_paralist.push_back(Symbol(token2SymbolType(type), ident.getValue()));
				ir.irShow(FORMAT("para {} {};", type.getValue(), ident.getValue()));
				// Group
				paraListGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				ERROR
			}
		}
		// ＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
		void mainFunc() {
			switch (lookTokenType()) {
			case TokenType::VOIDTK:
				_status_func_defining_type = SymbolType::FUNC_VOID;
				// void main‘(’‘)’
				eatToken(TokenType::VOIDTK);
				eatToken(TokenType::MAINTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::RPARENT);
				// ‘{’＜复合语句＞‘}’
				eatToken(TokenType::LBRACE);
				ir.defineFunc("main");
				ir.appendInstr({ Instr::MOVE, "$fp", "$sp" });
				_symbol_table.pushScope();
				// debugln("exprPush at {}", getLineNumber());
				_para_cnt = 0;
				compStatement();
				eatToken(TokenType::RBRACE);
				_symbol_table.popScope();
				// debugln("pop at {}", getLineNumber());
				break;
			default:
				ERROR
			}
			syntaxOutput("<主函数>");
		}
		// ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
		tuple<SymbolType, std::string> expr() {
			// ir.exprStart();
			SymbolType expr_type = SymbolType::INT;
			Token token;
			Symbol symbol;

			bool has_group = false;
			
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				if (lookTokenType() == TokenType::IDENFR) {
					token = lookToken();
					tie(_find_success, symbol) = _symbol_table.findSymbol(token.getValue());
					checkFind();
					if (symbol.getType() == SymbolType::CHAR || symbol.getType() == SymbolType::FUNC_CHAR) {
						expr_type = SymbolType::CHAR;
					}
				}
				if (lookTokenType() == TokenType::CHARCON) {
					expr_type = SymbolType::CHAR;

				}
				//
				// 
				// ［＋｜－］
				exprPrefix();
				// ＜项＞
				has_group = term() || has_group;
				// {＜加法运算符＞＜项＞} 
				has_group = exprGroup() || has_group;
				if (has_group)
					expr_type = SymbolType::INT;
				break;
			default:
				ERROR
			}
			
			syntaxOutput("<表达式>");
			return make_tuple(expr_type, ir.gen());
		}
		// // ［＋｜－］
		void exprPrefix() {
			switch (lookTokenType()) {
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				ir.exprPush(IR::NEG);
				break;
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::LPARENT:
				return;
				break;
			default:
				ERROR
			}
		}
		// {＜加法运算符＞＜项＞} 
		bool exprGroup() {
			Token op_token;
			
			switch (lookTokenType()) {
			case TokenType::PLUS:
			case TokenType::MINU:
				// ＜加法运算符＞
				op_token = plusOp();
				if (op_token.getTokenType() == TokenType::PLUS) {
					ir.exprPush(IR::PLUS);
				} else {
					ir.exprPush(IR::MINUS);
				}
				// ＜项＞
				term();
				// Group
				exprGroup();
				break;
			case TokenType::LSS:
			case TokenType::LEQ:
			case TokenType::GEQ:
			case TokenType::GRE:
			case TokenType::EQL:
			case TokenType::NEQ:
			case TokenType::SEMICN:
			case TokenType::COMMA:
			case TokenType::RPARENT:
			case TokenType::RBRACK:
				return false;
			default:
				ERROR
			}
			return true;
		}
		// ＜项＞::= ＜因子＞{＜乘法运算符＞＜因子＞}
		bool term() {
			bool has_group = false;
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				//  ＜因子＞
				factor();
				// {＜乘法运算符＞＜因子＞}
				has_group = termGroup();
				break;
			default:
				ERROR
			}
			syntaxOutput("<项>");
			return has_group;
		}
		// {＜乘法运算符＞＜因子＞}
		bool termGroup() {
			switch (lookTokenType()) {
			case TokenType::MULT:
			case TokenType::DIV:
				// ＜乘法运算符＞
				mulOp();
				// ＜因子＞
				factor();
				// Group
				termGroup();
				break;
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LSS:
			case TokenType::LEQ:
			case TokenType::GEQ:
			case TokenType::GRE:
			case TokenType::EQL:
			case TokenType::NEQ:
			case TokenType::SEMICN:
			case TokenType::COMMA:
			case TokenType::RPARENT:
			case TokenType::RBRACK:
				return false;
			default:
				ERROR
			}
			return true;
		}

		bool isInA(int offset) {
			return -offset / 4 < 4;
		}

		std::string offset2rega(int offset) {
			return FORMAT("$a{}", -offset / 4);
		}

		
		// ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'
		//					|'('＜表达式＞')'｜＜整数＞|＜字符＞
		//				    ｜＜有返回值函数调用语句＞         
		void factor() {
			SymbolType expr_type;
			Token token;

			Token int_token, char_token;
			std::string expr_ans, addr;
			
			switch (lookTokenType()) {
			case TokenType::IDENFR:
				switch (lookTokenType(1)) {
				case TokenType::LPARENT:
					// ＜有返回值函数调用语句＞       
					funcCall();
					ir.exprPushReg("$v0");
					break;
				case TokenType::LBRACK:
				case TokenType::LEQ:
				case TokenType::NEQ:
				case TokenType::RPARENT:
				case TokenType::EQL:
				case TokenType::LSS:
				case TokenType::MINU:
				case TokenType::GEQ:
				case TokenType::COMMA:
				case TokenType::GRE:
				case TokenType::MULT:
				case TokenType::DIV:
				case TokenType::PLUS:
				case TokenType::RBRACK:
				case TokenType::SEMICN:
					// ＜标识符＞
					token = eatToken(TokenType::IDENFR);
					
					switch (lookTokenType()) {
					case TokenType::LBRACK:
						// '['＜表达式＞']'
						eatToken(TokenType::LBRACK);
						// ir.notGen();
						ir.exprStart();
						tie(expr_type, expr_ans) = expr();
						if (expr_type != SymbolType::INT) {
							error('i');
						}
						eatToken(TokenType::RBRACK);

						if (_symbol_table.isGlobal(token.getValue())) {
							ir.exprPushStackArrGlo(_symbol_table.getGlobalBytesByIdent(token.getValue()), expr_ans);
						} else {
							ir.exprPushStackArrSta(_symbol_table.getStackBytesByIdent(token.getValue()), expr_ans);
						}
						ir.instrShowAs(token.getValue());
						
						ir.instrNotShow();
						
						break;
					case TokenType::RBRACK:
					case TokenType::PLUS:
					case TokenType::MINU:
					case TokenType::MULT:
					case TokenType::DIV:
					case TokenType::LSS:
					case TokenType::LEQ:
					case TokenType::GEQ:
					case TokenType::GRE:
					case TokenType::EQL:
					case TokenType::NEQ:
					case TokenType::SEMICN:
					case TokenType::COMMA:
					case TokenType::RPARENT:
						// Not Array
						// ir.loadStack()
						if (_symbol_table.isConst(token.getValue())) {
							// if (_symbol_table.isChar(token.getValue())) {
								// ir.exprPushLiteralInt(int(_symbol_table.getConstValue(token.getValue())[0]));
							// } else {
								ir.exprPushLiteralInt(a2i(_symbol_table.getConstValue(token.getValue())));
							// }
						}
						else {
							exprPushVar(token);
						}
						// 	if (_symbol_table.isGlobal(token.getValue())) {
						// 	ir.exprPushGlobalVar(token.getValue());
						// }
						// else {
						// 	ir.exprPushStackVar(token.getValue(), _symbol_table.getStackBytesByIdent(token.getValue()));
						// 	ir.instrShowAs(token.getValue());
						// }
						break;
					default:
						ERROR
					}
					break;
				default:
					ERROR
				}
				break;
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::INTCON:
				// ＜整数＞
				tie(std::ignore, int_token) = integer();
				ir.exprPushLiteralInt(a2i(int_token.getValue()));
				break;
			case TokenType::CHARCON:
				// ＜字符＞
				char_token = eatToken(TokenType::CHARCON);
				ir.exprPushLiteralInt(int(char_token.getValue()[0]));
				break;
			case TokenType::LPARENT:
				// '('＜表达式＞')'
				eatToken(TokenType::LPARENT);
				ir.exprPush(IR::PARE_L);
				// ir.notGen();
				ir.exprStart();
				tie(expr_type, expr_ans) = expr();
				ir.exprPushReg(expr_ans);
				eatToken(TokenType::RPARENT);
				ir.exprPush(IR::PARE_R);
				break;
			default:
				ERROR
			}
			syntaxOutput("<因子>");
		}

		// ＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'| ＜有返回值函数调用语句＞; 
		//		| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞;
		void statement() {
			switch (lookTokenType()) {
			case TokenType::IDENFR:
				switch (lookTokenType(1)) {
				case TokenType::LPARENT:
					// ＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;
					funcCall();
					eatToken(TokenType::SEMICN);
					break;
				case TokenType::ASSIGN:
				case TokenType::LBRACK:
					// ＜赋值语句＞; 
					assignStat();
					eatToken(TokenType::SEMICN);
					break;
				default:
					ERROR
				}
				break;
			case TokenType::IFTK:
				// ＜条件语句＞
				condStat();
				break;
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
				// ＜循环语句＞
				cycleStat();
				break;
			case TokenType::SCANFTK:
				// ＜读语句＞;
				scanfStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::PRINTFTK:
				// ＜写语句＞; 
				printfStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::RETURNTK:
				// ＜返回语句＞;
				retStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::SEMICN:
				// ＜空＞;
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::LBRACE:
				// '{'＜语句列＞'}'
				eatToken(TokenType::LBRACE);
				// _symbol_table.pushScope();
				statementCol();
				resetOuput();
				eatToken(TokenType::RBRACE);
				// _symbol_table.popScope();
				break;
			default:
				ERROR
			}
			syntaxOutput("<语句>");
		}
	
		void assign(std::string ident, std::string ans) {
			if (_symbol_table.isGlobal(ident)) {
				ir.appendInstr(Instr(Instr::SAVE_GLO, ans, _symbol_table.getGlobalBytesByIdent(ident)));
				ir.instrShowAs(ident);
			} else {
				ir.appendInstr(Instr(Instr::SAVE_STA, ans, _symbol_table.getStackBytesByIdent(ident)));
				ir.instrShowAs(ident);
			}
		}
		
		// ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
		void assignStat() {
			Token ident;
			Symbol symbol;

			SymbolType expr_type;
			std::string expr_reg, expr_ans_lhs, expr_ans_rhs;

			
			switch (lookTokenType()) {
			case TokenType::IDENFR:
				
				//  ＜标识符＞
				ident = eatToken(TokenType::IDENFR);
				tie(_find_success, symbol) = _symbol_table.findSymbol(ident.getValue());
				checkFind();
				if (symbol.isConst()) {
					error('j');
				}
				
				switch (lookTokenType()) {
				case TokenType::ASSIGN:
					// ＝＜表达式＞
					eatToken(TokenType::ASSIGN);
					ir.exprStart();
					tie(std::ignore, expr_reg) = expr();
					
					assign(ident.getValue(), expr_reg);
					break;
				case TokenType::LBRACK:
					// '['＜表达式＞']'=＜表达式＞
					eatToken(TokenType::LBRACK);
					ir.exprStart();
					tie(expr_type, expr_ans_lhs) = expr();
					if (expr_type != SymbolType::INT) {
						error('i');
					}

					eatToken(TokenType::RBRACK);
					eatToken(TokenType::ASSIGN);
					ir.exprStart();
					tie(expr_type, expr_ans_rhs) = expr();

					if (_symbol_table.isGlobal(ident.getValue())) {
						ir.appendInstr({ Instr::SAVE_ARR_GLO, expr_ans_rhs, _symbol_table.getGlobalBytesByIdent(ident.getValue()), expr_ans_lhs });
					} else {
						ir.appendInstr({ Instr::SAVE_ARR_STA, expr_ans_rhs, _symbol_table.getStackBytesByIdent(ident.getValue()), expr_ans_lhs });
					}
					ir.instrShowAs(ident.getValue());
					
					break;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
			syntaxOutput("<赋值语句>");
		}
		// ＜条件语句＞  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
		void condStat() {
			switch (lookTokenType()) {
			case TokenType::IFTK:
				// if '('＜条件＞')'＜语句＞
				eatToken(TokenType::IFTK);
				ir.newIf();
				ir.newBlock(ir.getIfName());
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				ir.newBlock(ir.getIfThanName());
				statement();
				// ［else＜语句＞］
				ir.jump(ir.getIfEndName());
				ir.newBlock(ir.getIfElseName());;
				condStatElse();
				ir.newBlock(ir.getIfEndName());
				ir.endJumpDefine();
				break;
			default:
				ERROR
			}
			syntaxOutput("<条件语句>");
		}
		// ［else＜语句＞］
		void condStatElse() {
			switch (lookTokenType()) {
			case TokenType::ELSETK:
				// else＜语句＞
				eatToken(TokenType::ELSETK);
				statement();
				break;
			// Empty
			case TokenType::IDENFR:
			case TokenType::IFTK:
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
			case TokenType::SCANFTK:
			case TokenType::PRINTFTK:
			case TokenType::RETURNTK:
			case TokenType::SEMICN:
			case TokenType::LBRACE:
			case TokenType::RBRACE:
				return;
				break;
			default:
				ERROR
			}
		}
		// ＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞
		//				｜＜表达式＞ //表达式为0条件为假，否则为真
		void cond() {
			SymbolType expr_type_lhs;
			SymbolType expr_type_rhs;
			Token cmp_token;
			TokenType cmp_type;
			std::string expr_ans_lhs, expr_ans_rhs;
			
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				// ＜表达式＞
				ir.exprStart();
				tie(expr_type_lhs, expr_ans_lhs) = expr();

				if (expr_type_lhs != SymbolType::INT) {
					error('f');
				}
				
				switch (lookTokenType()) {
				case TokenType::LSS:
				case TokenType::LEQ:
				case TokenType::GEQ:
				case TokenType::GRE:
				case TokenType::EQL:
				case TokenType::NEQ:
					//＜关系运算符＞＜表达式＞
					cmp_token = relationOp();
					ir.exprStart();
					tie(expr_type_rhs, expr_ans_rhs) = expr();
					
					cmp_type = cmp_token.getTokenType();
						
					if (expr_type_rhs != SymbolType::INT) {
						error('f');
					}

					switch (cmp_token.getTokenType().type_) {
					case TokenType::LSS:
						ir.appendInstr({ Instr::BGE, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					case TokenType::LEQ:
						ir.appendInstr({ Instr::BGT, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					case TokenType::GEQ:
						ir.appendInstr({ Instr::BLT, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					case TokenType::GRE:
						ir.appendInstr({ Instr::BLE, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					case TokenType::EQL:
						ir.appendInstr({ Instr::BNE, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					case TokenType::NEQ:
						ir.appendInstr({ Instr::BEQ, expr_ans_lhs, expr_ans_rhs, ir.getCondJumpName() });
						break;
					}
					
					break;
				case TokenType::SEMICN:
				case TokenType::RPARENT:
					// Emtpy
					ir.appendInstr({ Instr::BLEZ, expr_ans_lhs, ir.getCondJumpName() });
					break;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
			syntaxOutput("<条件>");
		}

		void exprPushVar(Token token) {
			if (_symbol_table.isGlobal(token.getValue())) {
				ir.exprPushGlobalVar(token.getValue(), _symbol_table.getGlobalBytesByIdent(token.getValue()));
			}
			else {
				ir.exprPushStackVar(token.getValue(), _symbol_table.getStackBytesByIdent(token.getValue()));
			}
		}

		void saveVar(std::string expr_reg, Token ident) {
			assign(ident.getValue(), expr_reg);
		}
		
		// ＜循环语句＞   ::=  while '('＜条件＞')'＜语句＞
		// | do＜语句＞while '('＜条件＞')'
		// |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
		
		void cycleStat() {
			// Token 
			Token while_token;

			Token for_step_op;
			Token for_step;
			Token for_step_ident_lhs, for_step_ident_rhs;
			std::string step_str;
			Token for_start_token;
			std::string for_start_ans;
			
			switch (lookTokenType()) {
			case TokenType::DOTK:
				// do＜语句＞while '('＜条件＞')'
				ir.newDoWhile();
				ir.newBlock(ir.getDoWhileName());
				eatToken(TokenType::DOTK);
				statement();
				while_token = eatToken(TokenType::WHILETK, 'n');
				if (while_token.getTokenType() == TokenType::ERR) {
					jumpUntil({ TokenType::SEMICN });
					return;
				}
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				ir.jump(ir.getDoWhileName());
				ir.newBlock(ir.getDoWhileEndName());
				ir.endJumpDefine();
				break;
			case TokenType::WHILETK:
				//  while '('＜条件＞')'＜语句＞
				ir.newWhile();
				ir.newBlock(ir.getWhileName());
				eatToken(TokenType::WHILETK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				statement();
				ir.jump(ir.getWhileName());
				ir.newBlock(ir.getWhileEndName());
				ir.endJumpDefine();
				break;
			case TokenType::FORTK:
				// for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
				eatToken(TokenType::FORTK);
				eatToken(TokenType::LPARENT);
				for_start_token = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				ir.exprStart();
				tie(std::ignore, for_start_ans) = expr();
				saveVar(for_start_ans, for_start_token);
				eatToken(TokenType::SEMICN);
				ir.newFor();
				ir.newBlock(ir.getForStartName());
				cond();
				eatToken(TokenType::SEMICN);
				for_step_ident_lhs = eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				for_step_ident_rhs = eatToken(TokenType::IDENFR);
				for_step_op = plusOp();
				for_step = step();
				eatToken(TokenType::RPARENT);
				ir.newBlock(ir.getForBodyName());
				statement();

				ir.exprStart();
				step_str = for_step.getValue();
				exprPushVar(for_step_ident_rhs);
				if (for_step_op.getTokenType().type_ == TokenType::PLUS) {
					ir.exprPush(IR::PLUS);
				} else {
					ir.exprPush(IR::MINUS);
				}
				ir.exprPushLiteralInt(a2i(step_str));
				saveVar(ir.gen(), for_step_ident_lhs);
				
				ir.jump(ir.getForStartName());
				ir.newBlock(ir.getForEndName());
				ir.endJumpDefine();
				break;
			default:
				ERROR
			}
			syntaxOutput("<循环语句>");
		}
		// ＜步长＞::= ＜无符号整数＞  
		Token step() {
			Token token;
			switch (lookTokenType()) {
			case TokenType::INTCON:
				token = uninteger();
				break;
			default:
				ERROR
			}
			syntaxOutput("<步长>");
			return token;
		}

		// void pushRegPool() {
		// 	// TODO: repair, move to mips
		// 	for (int i = 0; i <= POOLSIZE; i++) {
		// 		ir.appendInstr({ Instr::LOAD_LAB_IMM, "$k0", "REGPOOL", 4 * i });
		// 		ir.instrNotShow();
		// 		pushStackReg("$k0");
		// 		ir.instrNotShow();
		// 	}
		// }
		//
		// void popRegPool() {
		// 	for (int i = POOLSIZE; i >= 0; i--) {
		// 		popStackReg("$k0");
		// 		ir.instrNotShow();
		// 		ir.appendInstr({ Instr::SAVE_LAB_IMM, "$k0", "REGPOOL", 4 * i });
		// 		ir.instrNotShow();
		// 	}
		// }
		
		// ＜有/无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
		void funcCall() {
			Token name;
			std::vector<SymbolType> val_para_list, excepted_para_list;
			Symbol func_symbol;
			
			pushStackReg("$fp");

			ir.appendInstr({ Instr::PUSH_REGPOOL });
			// pushRegPool();

			// ir.appendInstr({ Instr::MINUS, "$k1", "$sp", "4"});
			// pushStackReg("$k1");
			
			
			switch (lookTokenType()) {
			case TokenType::IDENFR:
				name = eatToken(TokenType::IDENFR);
				tie(_find_success, func_symbol) = _symbol_table.findSymbol(name.getValue());
				checkFind();
				
				eatToken(TokenType::LPARENT);
				val_para_list = valParaList(func_symbol.getParaList());
				// _symbol_table.subOffset(val_para_list.size() * 4);
				excepted_para_list = func_symbol.getParaList();
				if (excepted_para_list != val_para_list) {
					// int length = std::min(excepted_para_list.size(), val_para_list.size());
					// for (int i = 0; i < length; i++) {
					// 	if (excepted_para_list[i] != val_para_list[i]) {
					// 		error('e');
					// 		break;
					// 	}
					// }
					if (excepted_para_list.size() != val_para_list.size()) {
						error('d');
					}
					// panic("aho");
				}

				eatToken(TokenType::RPARENT);
				break;
			default:
				ERROR
			}

			
			// popStackReg("$k1");
			ir.appendInstr({ Instr::PLUS, "$fp", "$sp", int(val_para_list.size() * 4) });

			
			ir.call(name.getValue());
			ir.newBlock(FORMAT("after_call_{}_{}", name.getValue(), ir.newLabelCnt()));

			// popRegPool();
			ir.appendInstr({ Instr::POP_REGPOOL });

			popStackReg("$fp");

			// // debugln("!{}", name.getValue());
			if (funcName2IsRet[name.getValue()]) {
				syntaxOutput("<有返回值函数调用语句>");
			} else {
				syntaxOutput("<无返回值函数调用语句>");
			}
		}
		std::vector<SymbolType> _val_para_list{};
		// ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
		std::vector<SymbolType> valParaList(std::vector<SymbolType> expected_symbols) {
			int index = 0;
			std::vector<SymbolType> backup = _val_para_list;
			_val_para_list.clear();

			std::string expr_ans_reg;
			
			SymbolType expr_type;
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				// ＜表达式＞
				ir.exprStart();
				tie(expr_type, expr_ans_reg) = expr();
				pushStackReg(expr_ans_reg);
#ifdef STANDARD_REG_A
				ir.appendInstr({ Instr::MOVE, "$a0", expr_ans_reg });
#endif
				_val_para_list.push_back(expr_type);
				if (expected_symbols.size() > index && expected_symbols[index] != expr_type) {
					error('e');
				}
				// {,＜表达式＞}
				valParaListGroup(expected_symbols, index+1);
				break;
			case TokenType::RPARENT:
				// ＜空＞
				break;
			default:
				ERROR
			}
			// ir.popStack(_val_para_list.size()*4);
			syntaxOutput("<值参数表>");
			std::vector<SymbolType> ret = _val_para_list;
			_val_para_list = backup;
			// if (func_type == SymbolType::FUNC_INT) {
			// 	_val_para_list.push_back(SymbolType::INT);
			// } else if (func_type == SymbolType::FUNC_CHAR) {
			// 	_val_para_list.push_back(SymbolType::CHAR);
			// } else {
			// 	assert(func_type == SymbolType::FUNC_VOID);
			// }
			return ret;
		}
		// {,＜表达式＞}
		void valParaListGroup(std::vector<SymbolType> expected_symbols, int index) {
			SymbolType expr_type;
			std::string expr_ans_reg;
			
			switch (lookTokenType()) {
			case TokenType::COMMA:
				// ,＜表达式＞
				eatToken(TokenType::COMMA);
				ir.exprStart();
				tie(expr_type, expr_ans_reg) = expr();
				
				pushStackReg(expr_ans_reg);
#ifdef STANDARD_REG_A
				if (index < 4) {
					ir.appendInstr({ Instr::MOVE, FORMAT("$a{}", index), expr_ans_reg });
				}
#endif
				_val_para_list.push_back(expr_type);
				if (expected_symbols.size() > index && expected_symbols[index] != expr_type) {
					error('e');
				}
				// Group
				valParaListGroup(expected_symbols, index+1);
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				ERROR
			}
		}
		// ＜语句列＞ ::= ｛＜语句＞｝
		void statementCol() {
			switch (lookTokenType()) {
			case TokenType::IDENFR:
			case TokenType::IFTK:
			case TokenType::DOTK:
			case TokenType::WHILETK:
			case TokenType::FORTK:
			case TokenType::SCANFTK:
			case TokenType::PRINTFTK:
			case TokenType::RETURNTK:
			case TokenType::SEMICN:
			case TokenType::LBRACE:
				// 语句
				statement();
				// Group
				statementCol();
				break;
			case TokenType::RBRACE:
				// return;
				break;
			default:
				ERROR
			}
			syntaxOutput("<语句列>");
		}
		// ＜读语句＞    ::=  scanf '('＜标识符＞{,＜标识符＞}')'
		void scanfStat() {
			Token ident_token;

			int scope;
			Symbol symbol;
			int i;
			
			switch (lookTokenType()) {
			case TokenType::SCANFTK:
				// scanf '('＜标识符＞
				eatToken(TokenType::SCANFTK);
				eatToken(TokenType::LPARENT);
				_status_ident_group.clear();

				ident_token = eatToken(TokenType::IDENFR);
				_status_ident_group.push_back(ident_token.getValue());

				// {,＜标识符＞}
				identGroup();

				// tie(scope, symbol) = _symbol_table.findSymbolAndScope(ident_token.getValue());
				// if (symbol.isGlobal()) {
				// 	ir.appendInstr({ Instr::SCAN_GLOBAL_INT, ir.getGlobalName(symbol.getIdent()) });
				// } else {
				// 	ir.appendInstr({ Instr::SCAN_INT, _symbol_table.getStackBytesByIdent(symbol.getIdent()) });
				// }
				for (i = 0; i < _status_ident_group.size(); i++) {
					tie(scope, symbol) = _symbol_table.findSymbolAndScope(_status_ident_group[i]);
					if (symbol.isGlobal()) {
						if (symbol.getType().type_ == SymbolType::INT) {
							ir.appendInstr({ Instr::SCAN_GLOBAL_INT, _symbol_table.getGlobalBytesByIdent(symbol.getIdent()) });
							ir.instrShowAs(symbol.getIdent());
						} else if (symbol.getType().type_ == SymbolType::CHAR) {
							ir.appendInstr({ Instr::SCAN_GLOBAL_CHAR, _symbol_table.getGlobalBytesByIdent(symbol.getIdent()) });
							ir.instrShowAs(symbol.getIdent());
						}
					}
					else {
						if (symbol.getType().type_ == SymbolType::INT) {
							ir.appendInstr({ Instr::SCAN_INT, _symbol_table.getStackBytesByIdent(symbol.getIdent()) });
							ir.instrShowAs(symbol.getIdent());
						}
						else if (symbol.getType().type_ == SymbolType::CHAR) {
							ir.appendInstr({ Instr::SCAN_CHAR, _symbol_table.getStackBytesByIdent(symbol.getIdent()) });
							ir.instrShowAs(symbol.getIdent());
						}
					}
				}
				
				
				// ')'
				eatToken(TokenType::RPARENT);
				break;
			default:
				ERROR
			}
			syntaxOutput("<读语句>");
		}
		std::vector<std::string> _status_ident_group;
		// {,＜标识符＞}
		void identGroup() {
			Token ident_token;
			switch (lookTokenType()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				ident_token = eatToken(TokenType::IDENFR);
				_status_ident_group.push_back(ident_token.getValue());
				identGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				ERROR
			}
		}
		// ＜写语句＞    ::= printf '(' ＜字符串＞,＜表达式＞ ')'
		//	| printf '('＜字符串＞ ')'
		//	| printf '('＜表达式＞')'
		void printfStat() {
			std::string str;
			std::string str_label;

			SymbolType expr_type;
			std::string expr_ans;
			
			switch (lookTokenType()) {
			case TokenType::PRINTFTK:
				eatToken(TokenType::PRINTFTK);
				eatToken(TokenType::LPARENT);
				switch (lookTokenType()) {
				case TokenType::STRCON:
					str = strconst();
					str_label = ir.defineConstStr( str );
					ir.printGlobalStr(str_label);
					switch (lookTokenType()) {
					case TokenType::COMMA:
						eatToken(TokenType::COMMA);
						ir.exprStart();
						tie(expr_type, expr_ans) = expr();
						if (expr_type.type_ == SymbolType::INT) {
							ir.appendInstr({ Instr::PRINT_INT, expr_ans });
						}
						else if (expr_type.type_ == SymbolType::CHAR) {
							ir.appendInstr({ Instr::PRINT_CHAR, expr_ans });
						}
						eatToken(TokenType::RPARENT);
						break;
					case TokenType::RPARENT:
						eatToken(TokenType::RPARENT);
						break;
					default:
						ERROR
					}
					break;
				case TokenType::IDENFR:
				case TokenType::INTCON:
				case TokenType::CHARCON:
				case TokenType::PLUS:
				case TokenType::MINU:
				case TokenType::LPARENT:
					ir.exprStart();
					tie(expr_type, expr_ans) = expr();
					if (expr_type.type_ == SymbolType::INT) {
						ir.appendInstr({ Instr::PRINT_INT, expr_ans });
					} else if (expr_type.type_ == SymbolType::CHAR) {
						ir.appendInstr({ Instr::PRINT_CHAR, expr_ans });
					}
					eatToken(TokenType::RPARENT);
					break;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
			ir.printLine();
			syntaxOutput("<写语句>");
		}
		// ＜返回语句＞   ::=  return['('＜表达式＞')']    
		void retStat() {
			SymbolType expr_type;
			std::string expr_ans;
			
			switch (lookTokenType()) {
			case TokenType::RETURNTK:
				eatToken(TokenType::RETURNTK);
				switch (lookTokenType()) {
				case TokenType::LPARENT:
					// '('＜表达式＞')'
					eatToken(TokenType::LPARENT);
					ir.exprStart();
					tie(expr_type, expr_ans) = expr();
					ir.moveReg("$v0", expr_ans);
					ir.jump(ir.getReturnLabel());
					ir.newBlock(FORMAT("after_jump_{}_{}", ir.getReturnLabel(), ir.newLabelCnt()));
					_status_value_ret_cnt++;
					if (_status_func_defining_type == SymbolType::FUNC_INT && expr_type != SymbolType::INT) {
						error('h');
					}
					if (_status_func_defining_type == SymbolType::FUNC_CHAR && expr_type != SymbolType::CHAR) {
						error('h');
					}
					if (_status_func_defining_type == SymbolType::FUNC_VOID) {
						error('g');
					}
					eatToken(TokenType::RPARENT);
					break;
				case TokenType::SEMICN:
					// Empty;
					if (_status_func_defining_type != SymbolType::FUNC_VOID) {
						error('h');
					}
					ir.jump(ir.getReturnLabel());
					break;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
			syntaxOutput("<返回语句>");
		}


#pragma endregion 
		
		TokenType::Type lookTokenType(int ahead = 0) {
			return lex_parser_.lookToken(ahead).unwrap().getTokenType().type_;
		}

		Token lookToken(int ahead = 0) {
			return lex_parser_.lookToken(ahead).unwrap();
		}

		Token eatToken(TokenType excepted_token_type, char code = '\0') {
			Token token = lex_parser_.eatToken().unwrap();
			if (token.getTokenType().type_ != excepted_token_type.type_) {
				switch (excepted_token_type.type_) {
				case TokenType::SEMICN:
					error('k');
					break;
				case TokenType::RPARENT:
					error('l');
					break;
				case TokenType::RBRACK:
					error('m');
					break;
				default:
					if (code != '\0') {
						error(code);
						return Token(TokenType::ERR);
					}
					
					panic("unimplemented");
				}
			}
			return token;
		}

		Token jumpToken() {
			Token token = lex_parser_.eatToken().unwrap();
			return token;
		}

		std::vector<char> _error_status{};
		
		void errorByStatus() {
			if (!_error_status.empty())
				error(_error_status.back());
		}

		void pushErrorStatus(char code) {
			_error_status.push_back(code);
		}

		void popErrorStatus(char code) {
			if (_error_status.back() != code) {
				panic("error status error");
			}
			_error_status.pop_back();
		}
		
		void error(char code) {
			if (output_error_) std::cout << getLineNumber() << " " << code << std::endl;
			// panic("aho");

		}
		
		void error() {
			panic("unimplemented");
		}

		SymbolType token2SymbolType(Token token, bool is_func = false) {
			if (token.getTokenType() == TokenType::INTTK) {
				return is_func ? SymbolType::FUNC_INT : SymbolType::INT;
			}
			else if (token.getTokenType() == TokenType::CHARTK) {
				return is_func ? SymbolType::FUNC_CHAR : SymbolType::CHAR;
			}
			else {
				panic("aho");
			}
		}

		std::vector<SymbolType> symbolList2SymbolTypeList(std::vector<Symbol> symbols) {
			std::vector<SymbolType> symbol_types;
			for (int i = 0; i < symbols.size(); i++) {
				symbol_types.push_back(symbols[i].getType());
			}
			return symbol_types;
		}
		
		lex::LexParser& lex_parser_;


		bool exceptTokens(std::vector<TokenType> tokens) {
			TokenType token = lookTokenType();
			for (int i = 0; i < tokens.size(); i++) {
				if (token == tokens[i]) {
					return true;
				}
			}
			return false;
		}

		void jumpUntil(std::vector<TokenType> tokens) {
			while (!exceptTokens(tokens)) {
				jumpToken();
			}
		}
		
	};

}
}

#endif
