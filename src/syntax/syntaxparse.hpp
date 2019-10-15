#ifndef BUAAC_SYNTAX_PARSE_HPP
#define BUAAC_SYNTAX_PARSE_HPP

#include "../meow/core.hpp"
#include "../lex/lexparse.hpp"
#include <map>

#define ERROR break;	// ignore all error

namespace buaac {
namespace syntax{
	
	class SyntaxParser {

		using Token = lex::Token;
		using TokenType = lex::TokenType;
		
	public:
		SyntaxParser(lex::LexParser& lex_parser, bool output = true): lex_parser_(lex_parser), output_(output) {

		}

		void start() {
			program();
		}

	private:

		std::map<std::string, bool> funcName2IsRet;

		bool stack_output_constDec_ = false;
		bool stack_output_verDec_ = false;
		bool stack_output_statementCol_ = false;

		bool output_ = false;

		void resetOuput() {
			stack_output_constDec_ = false;
			stack_output_verDec_ = false;
			stack_output_statementCol_ = false;
		}
		
		void syntaxOutput(std::string output) {
			if (!output_)
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
		void plusOp() {
			switch (lookToken()) {
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				break;
			default:
				ERROR
			}
		}
		// ＜乘法运算符＞  ::= *｜/
		void mulOp() {
			switch (lookToken()) {
			case TokenType::MULT:
				eatToken(TokenType::MULT);
				break;
			case TokenType::DIV:
				eatToken(TokenType::DIV);
				break;
			default:
				ERROR
			}
		}
		// ＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
		void relationOp() {
			switch (lookToken()) {
			case TokenType::LSS:
				eatToken(TokenType::LSS);
				break;
			case TokenType::LEQ:
				eatToken(TokenType::LEQ);
				break;
			case TokenType::GEQ:
				eatToken(TokenType::GEQ);
				break;
			case TokenType::GRE:
				eatToken(TokenType::GRE);
				break;
			case TokenType::EQL:
				eatToken(TokenType::EQL);
				break;
			case TokenType::NEQ:
				eatToken(TokenType::NEQ);
				break;
			default:
				ERROR
			}
		}
		// ＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
		void strconst() {
			switch (lookToken()) {
			case TokenType::STRCON:
				eatToken(TokenType::STRCON);
				break;
			default:
				ERROR
			}
			syntaxOutput("<字符串>");
		}
		// ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
		void program() {
			switch (lookToken()) {
			case TokenType::CONSTTK:
			case TokenType::INTTK:
			case TokenType::CHARTK:
			case TokenType::VOIDTK:
				constDec();
				resetOuput();
				verDec();
				resetOuput();
				funcDefGroup();
				mainFunc();
				break;
			default:
				ERROR
			}
			syntaxOutput("<程序>");
		}
		// {＜有返回值函数定义＞|＜无返回值函数定义＞}
		void funcDefGroup() {
			switch (lookToken()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				funcDef();
				funcDefGroup();
				break;
			case TokenType::VOIDTK:
				switch (lookToken(1)) {
				case TokenType::IDENFR:
					funcDef();
					funcDefGroup();
					break;
				case TokenType::MAINTK:
					return;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
		}
		// ＜有返回值函数定义＞|＜无返回值函数定义＞
		void funcDef() {
			switch (lookToken()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				retFuncDef();
				break;
			case TokenType::VOIDTK:
				nonRetFuncDef();
				break;
			default:
				ERROR
			}
		}
		// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
		void constDec() {
			switch (lookToken()) {
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
			switch (lookToken()) {
			case TokenType::INTTK:
				constDefInt();
				constDefIntGroup();
				break;
			case TokenType::CHARTK:
				constDefChar();
				constDefCharGroup();
				break;
			default:
				ERROR
			}
			syntaxOutput("<常量定义>");
		}
		// ＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝| 0
		void uninteger() {
			switch (lookToken()) {
			case TokenType::INTCON:
				eatToken(TokenType::INTCON);
				break;
			default:
				ERROR
			}
			syntaxOutput("<无符号整数>");
		}
		// ＜整数＞        ::= ［＋｜－］＜无符号整数＞
		void integer() {
			switch (lookToken()) {
			case TokenType::INTCON:
				uninteger();
				break;
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				uninteger();
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				uninteger();
				break;
			default:
				ERROR
			}
			syntaxOutput("<整数>");
		}
		void constDefInt() {
			switch (lookToken()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				integer();
				break;
			default:
				ERROR
			}
		}
		// {,＜标识符＞＝＜整数＞}
		void constDefIntGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				integer();
				constDefIntGroup();
				break;
			// STOP
			case TokenType::SEMICN:
				return;
			default:
				ERROR
			}
		}
		void constDefChar() {
			switch (lookToken()) {
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::CHARCON);
				break;
			default:
				ERROR
			}
		}
		// { ,＜标识符＞＝＜字符＞ }
		void constDefCharGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::CHARCON);
				constDefCharGroup();
				break;
			// STOP
			case TokenType::SEMICN:
				return;
			default:
				ERROR
			}
		}
		// ＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
		Token decHead() {
			Token idenfr;
			switch (lookToken()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				idenfr = eatToken(TokenType::IDENFR);
				break;
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				idenfr = eatToken(TokenType::IDENFR);
				break;
			default:
				ERROR
			}
			syntaxOutput("<声明头部>");
			return idenfr;
		}
		// ＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
		void verDec() {
			switch (lookToken()) {
			case TokenType::INTTK:
				switch (lookToken(2)) {
				case TokenType::SEMICN:
				case TokenType::COMMA:
				case TokenType::LBRACK:
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
			case TokenType::CHARTK:
				switch (lookToken(2)) {
				case TokenType::SEMICN:
				case TokenType::COMMA:
				case TokenType::LBRACK:
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
		
		//＜变量定义＞ ::= ＜类型标识符＞
		//				(＜标识符＞|＜标识符＞'['＜无符号整数＞']')
		//				{,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )} 
		void verDef() {
			switch (lookToken()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				// ＜类型标识符＞
				typeIdent();
				// (＜标识符＞|＜标识符＞'['＜无符号整数＞']')
				switch (lookToken()) {
				case TokenType::IDENFR:
					// ＜标识符＞
					eatToken(TokenType::IDENFR);
					switch (lookToken()) {
					case TokenType::LBRACK:
						// '['＜无符号整数＞']'
						eatToken(TokenType::LBRACK);
						uninteger();
						eatToken(TokenType::RBRACK);
						break;
					case TokenType::SEMICN:
					case TokenType::COMMA:
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

		// {,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )} 
		void verDefNameGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				// ,
				eatToken(TokenType::COMMA);
				switch (lookToken()) {
				case TokenType::IDENFR:
					// ＜标识符＞
					eatToken(TokenType::IDENFR);
					switch (lookToken()) {
					case TokenType::LBRACK:
						// '['＜无符号整数＞']' 
						eatToken(TokenType::LBRACK);
						uninteger();
						eatToken(TokenType::RBRACK);
						break;
					case TokenType::SEMICN:
					case TokenType::COMMA:
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
		void typeIdent() {
			switch (lookToken()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				break;
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				break;
			default:
				ERROR
			}
		}
		// ＜有返回值函数定义＞ ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
		void retFuncDef() {
			Token name;
			switch (lookToken()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				//  ＜声明头部＞
				name = decHead();
				funcName2IsRet[name.getValue()] = true;
				// '('＜参数表＞')' 
				eatToken(TokenType::LPARENT);
				paraList();
				eatToken(TokenType::RPARENT);
				// '{'＜复合语句＞'}'
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				ERROR
			}
			// debugln("!{}", name.getValue());
			syntaxOutput("<有返回值函数定义>");
		}
		// ＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
		void nonRetFuncDef() {
			Token name;
			switch (lookToken()) {
			case TokenType::VOIDTK:
				// void
				eatToken(TokenType::VOIDTK);
				// ＜标识符＞
				name = eatToken(TokenType::IDENFR);
				// '('＜参数表＞')'
				eatToken(TokenType::LPARENT);
				paraList();
				eatToken(TokenType::RPARENT);
				// '{'＜复合语句＞'}'
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				ERROR
			}
			// debugln("!{}", name.getValue());
			funcName2IsRet[name.getValue()] = false;
			syntaxOutput("<无返回值函数定义>");
		}

		// ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
		void compStatement() {
			switch (lookToken()) {
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
				resetOuput();
				// ＜语句列＞
				statementCol();
				resetOuput();
				break;
			default:
				ERROR
			}
			syntaxOutput("<复合语句>");
		}
		// ＜参数表＞::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
		void paraList() {
			switch (lookToken()) {
			case TokenType::INTTK:
			case TokenType::CHARTK:
				// ＜类型标识符＞
				typeIdent();
				// ＜标识符＞
				eatToken(TokenType::IDENFR);
				// {, ＜类型标识符＞＜标识符＞}
				paraListGroup();
				break;
			case TokenType::RPARENT:
				break;
			default:
				ERROR
			}
			syntaxOutput("<参数表>");
		}
		//  {, ＜类型标识符＞＜标识符＞}
		void paraListGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				// ,
				eatToken(TokenType::COMMA);
				// ＜类型标识符＞
				typeIdent();
				// ＜标识符＞
				eatToken(TokenType::IDENFR);
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
			switch (lookToken()) {
			case TokenType::VOIDTK:
				// void main‘(’‘)’
				eatToken(TokenType::VOIDTK);
				eatToken(TokenType::MAINTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::RPARENT);
				// ‘{’＜复合语句＞‘}’
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				ERROR
			}
			syntaxOutput("<主函数>");
		}
		// ＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞} 
		void expr() {
			switch (lookToken()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				// ［＋｜－］
				exprPrefix();
				// ＜项＞
				term();
				// {＜加法运算符＞＜项＞} 
				exprGroup();
				break;
			default:
				ERROR
			}
			syntaxOutput("<表达式>");
		}
		// // ［＋｜－］
		void exprPrefix() {
			switch (lookToken()) {
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
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
		void exprGroup() {
			switch (lookToken()) {
			case TokenType::PLUS:
			case TokenType::MINU:
				// ＜加法运算符＞
				plusOp();
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
				return;
				break;
			default:
				ERROR
			}
		}
		// ＜项＞::= ＜因子＞{＜乘法运算符＞＜因子＞}
		void term() {
			switch (lookToken()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				//  ＜因子＞
				factor();
				// {＜乘法运算符＞＜因子＞}
				termGroup();
				break;
			default:
				ERROR
			}
			syntaxOutput("<项>");
		}
		// {＜乘法运算符＞＜因子＞}
		void termGroup() {
			switch (lookToken()) {
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
			default:
				ERROR
			}
		}
		// ＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'
		//					|'('＜表达式＞')'｜＜整数＞|＜字符＞
		//				    ｜＜有返回值函数调用语句＞         
		void factor() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				switch (lookToken(1)) {
				case TokenType::LPARENT:
					// ＜有返回值函数调用语句＞       
					funcCall();
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
					eatToken(TokenType::IDENFR);
					switch (lookToken()) {
					case TokenType::LBRACK:
						// '['＜表达式＞']'
						eatToken(TokenType::LBRACK);
						expr();
						eatToken(TokenType::RBRACK);
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
				integer();
				break;
			case TokenType::CHARCON:
				// ＜字符＞
				eatToken(TokenType::CHARCON);
				break;
			case TokenType::LPARENT:
				// '('＜表达式＞')'
				eatToken(TokenType::LPARENT);
				expr();
				eatToken(TokenType::RPARENT);
				break;
			default:
				ERROR
			}
			syntaxOutput("<因子>");
		}

		// ＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'| ＜有返回值函数调用语句＞; 
		//		| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞;

		void statement() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				switch (lookToken(1)) {
				case TokenType::LPARENT:
					funcCall();
					eatToken(TokenType::SEMICN);
					break;
				case TokenType::ASSIGN:
				case TokenType::LBRACK:
					assignStat();
					eatToken(TokenType::SEMICN);
					break;
				default:
					ERROR
				}
				break;
			case TokenType::IFTK:
				condStat();
				break;
			case TokenType::DOTK:
				cycleStat();
				break;
			case TokenType::WHILETK:
				cycleStat();
				break;
			case TokenType::FORTK:
				cycleStat();
				break;
			case TokenType::SCANFTK:
				scanfStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::PRINTFTK:
				printfStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::RETURNTK:
				retStat();
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::SEMICN:
				eatToken(TokenType::SEMICN);
				break;
			case TokenType::LBRACE:
				eatToken(TokenType::LBRACE);
				statementCol();
				resetOuput();
				eatToken(TokenType::RBRACE);
				break;
			default:
				ERROR
			}
			syntaxOutput("<语句>");
		}
		
		// ＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
		void assignStat() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				//  ＜标识符＞
				eatToken(TokenType::IDENFR);
				switch (lookToken()) {
				case TokenType::ASSIGN:
					// ＝＜表达式＞
					eatToken(TokenType::ASSIGN);
					expr();
					break;
				case TokenType::LBRACK:
					// '['＜表达式＞']'=＜表达式＞
					eatToken(TokenType::LBRACK);
					expr();
					eatToken(TokenType::RBRACK);
					eatToken(TokenType::ASSIGN);
					expr();
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
			switch (lookToken()) {
			case TokenType::IFTK:
				// if '('＜条件＞')'＜语句＞
				eatToken(TokenType::IFTK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				statement();
				// ［else＜语句＞］
				condStatElse();
				break;
			default:
				ERROR
			}
			syntaxOutput("<条件语句>");
		}
		// ［else＜语句＞］
		void condStatElse() {
			switch (lookToken()) {
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
			switch (lookToken()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				// ＜表达式＞
				expr();
				switch (lookToken()) {
				case TokenType::LSS:
				case TokenType::LEQ:
				case TokenType::GEQ:
				case TokenType::GRE:
				case TokenType::EQL:
				case TokenType::NEQ:
					//＜关系运算符＞＜表达式＞
					relationOp();
					expr();
					break;
				case TokenType::SEMICN:
				case TokenType::RPARENT:
					// Emtpy
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
		// ＜循环语句＞   ::=  while '('＜条件＞')'＜语句＞
		// | do＜语句＞while '('＜条件＞')'
		// |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
		void cycleStat() {
			switch (lookToken()) {
			case TokenType::DOTK:
				// do＜语句＞while '('＜条件＞')'
				eatToken(TokenType::DOTK);
				statement();
				eatToken(TokenType::WHILETK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::WHILETK:
				//  while '('＜条件＞')'＜语句＞
				eatToken(TokenType::WHILETK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				statement();
				break;
			case TokenType::FORTK:
				// for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
				eatToken(TokenType::FORTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				expr();
				eatToken(TokenType::SEMICN);
				cond();
				eatToken(TokenType::SEMICN);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::IDENFR);
				plusOp();
				step();
				eatToken(TokenType::RPARENT);
				statement();
				break;
			default:
				ERROR
			}
			syntaxOutput("<循环语句>");
		}
		// ＜步长＞::= ＜无符号整数＞  
		void step() {
			switch (lookToken()) {
			case TokenType::INTCON:
				uninteger();
				break;
			default:
				ERROR
			}
			syntaxOutput("<步长>");
		}
		// ＜有/无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
		void funcCall() {
			Token name;
			switch (lookToken()) {
			case TokenType::IDENFR:
				name = eatToken(TokenType::IDENFR);
				eatToken(TokenType::LPARENT);
				valParaList();
				eatToken(TokenType::RPARENT);
				break;
			default:
				ERROR
			}
			// debugln("!{}", name.getValue());
			if (funcName2IsRet[name.getValue()]) {
				syntaxOutput("<有返回值函数调用语句>");
			} else {
				syntaxOutput("<无返回值函数调用语句>");
			}
		}
		// ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
		void valParaList() {
			switch (lookToken()) {
			case TokenType::IDENFR:
			case TokenType::INTCON:
			case TokenType::CHARCON:
			case TokenType::PLUS:
			case TokenType::MINU:
			case TokenType::LPARENT:
				// ＜表达式＞
				expr();
				// {,＜表达式＞}
				valParaListGroup();
				break;
			case TokenType::RPARENT:
				// ＜空＞
				break;
			default:
				ERROR
			}
			syntaxOutput("<值参数表>");
		}
		// {,＜表达式＞}
		void valParaListGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				// ,＜表达式＞
				eatToken(TokenType::COMMA);
				expr();
				// Group
				valParaListGroup();
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
			switch (lookToken()) {
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
			switch (lookToken()) {
			case TokenType::SCANFTK:
				// scanf '('＜标识符＞
				eatToken(TokenType::SCANFTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::IDENFR);
				// {,＜标识符＞}
				identGroup();
				// ')'
				eatToken(TokenType::RPARENT);
				break;
			default:
				ERROR
			}
			syntaxOutput("<读语句>");
		}
		// {,＜标识符＞}
		void identGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
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
			switch (lookToken()) {
			case TokenType::PRINTFTK:
				eatToken(TokenType::PRINTFTK);
				eatToken(TokenType::LPARENT);
				switch (lookToken()) {
				case TokenType::STRCON:
					strconst();
					switch (lookToken()) {
					case TokenType::COMMA:
						eatToken(TokenType::COMMA);
						expr();
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
					expr();
					eatToken(TokenType::RPARENT);
					break;
				default:
					ERROR
				}
				break;
			default:
				ERROR
			}
			syntaxOutput("<写语句>");
		}
		// ＜返回语句＞   ::=  return['('＜表达式＞')']    
		void retStat() {
			switch (lookToken()) {
			case TokenType::RETURNTK:
				eatToken(TokenType::RETURNTK);
				switch (lookToken()) {
				case TokenType::LPARENT:
					eatToken(TokenType::LPARENT);
					expr();
					eatToken(TokenType::RPARENT);
					break;
				case TokenType::SEMICN:
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
		
		TokenType::Type lookToken(int ahead = 0) {
			return lex_parser_.lookToken(ahead).unwrap().getTokenType().type_;
		}

		Token eatToken(TokenType excepted_token_type) {
			Token token = lex_parser_.eatToken().unwrap();
			if (token.getTokenType().type_ != excepted_token_type.type_) {
				panic("unimplemented");
			}
			return token;
		}

		void error() {
			panic("unimplemented");
		}
		
		lex::LexParser& lex_parser_;

	};

}
}

#endif
