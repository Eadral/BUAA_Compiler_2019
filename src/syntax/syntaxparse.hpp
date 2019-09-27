#ifndef BUAAC_SYNTAX_PARSE_HPP
#define BUAAC_SYNTAX_PARSE_HPP

#include "../meow/core.hpp"
#include "../lex/lexparse.hpp"

namespace buaac {
namespace syntax{
	
	class SyntaxParser {

		using Token = lex::Token;
		using TokenType = lex::TokenType;
		
	public:
		SyntaxParser(lex::LexParser& lex_parser): lex_parser_(lex_parser) {

		}

		void start() {
			program();
		}

	private:

		bool stack_output_constDec_ = false;
		bool stack_output_verDec_ = false;
		bool stack_output_statementCol_ = false;

		void syntaxOutput(std::string output) {
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
				stack_output_constDec_ = false;
				stack_output_verDec_ = false;
				stack_output_statementCol_ = false;
			}
		}
		
#pragma region GeneratedSyntax

		void plusOp() {
			switch (lookToken()) {
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				break;
			default:
				error();
			}
		}
		void mulOp() {
			switch (lookToken()) {
			case TokenType::MULT:
				eatToken(TokenType::MULT);
				break;
			case TokenType::DIV:
				eatToken(TokenType::DIV);
				break;
			default:
				error();
			}
		}
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
				error();
			}
		}
		void strconst() {
			switch (lookToken()) {
			case TokenType::STRCON:
				eatToken(TokenType::STRCON);
				break;
			default:
				error();
			}
			syntaxOutput("<字符串>");
		}
		void program() {
			switch (lookToken()) {
			case TokenType::CONSTTK:
				constDec();
				verDec();
				funcDefGroup();
				mainFunc();
				break;
			case TokenType::INTTK:
				constDec();
				verDec();
				funcDefGroup();
				mainFunc();
				break;
			case TokenType::CHARTK:
				constDec();
				verDec();
				funcDefGroup();
				mainFunc();
				break;
			case TokenType::VOIDTK:
				constDec();
				verDec();
				funcDefGroup();
				mainFunc();
				break;
			default:
				error();
			}
			syntaxOutput("<程序>");
		}
		void funcDefGroup() {
			switch (lookToken()) {
			case TokenType::INTTK:
				funcDef();
				funcDefGroup();
				break;
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
					error();
				}
				break;
			default:
				error();
			}
		}
		void funcDef() {
			switch (lookToken()) {
			case TokenType::INTTK:
				retFuncDef();
				break;
			case TokenType::CHARTK:
				retFuncDef();
				break;
			case TokenType::VOIDTK:
				nonRetFuncDef();
				break;
			default:
				error();
			}
		}
		void constDec() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				return;
				break;
			case TokenType::CONSTTK:
				eatToken(TokenType::CONSTTK);
				constDef();
				eatToken(TokenType::SEMICN);
				constDec();
				break;
			case TokenType::INTTK:
				return;
				break;
			case TokenType::CHARTK:
				return;
				break;
			case TokenType::VOIDTK:
				return;
				break;
			case TokenType::IFTK:
				return;
				break;
			case TokenType::DOTK:
				return;
				break;
			case TokenType::WHILETK:
				return;
				break;
			case TokenType::FORTK:
				return;
				break;
			case TokenType::SCANFTK:
				return;
				break;
			case TokenType::PRINTFTK:
				return;
				break;
			case TokenType::RETURNTK:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::LBRACE:
				return;
				break;
			case TokenType::RBRACE:
				return;
				break;
			default:
				error();
			}
			syntaxOutput("<常量说明>");
		}
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
				error();
			}
			syntaxOutput("<常量定义>");
		}
		void uninteger() {
			switch (lookToken()) {
			case TokenType::INTCON:
				eatToken(TokenType::INTCON);
				break;
			default:
				error();
			}
			syntaxOutput("<无符号整数>");
		}
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
				error();
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
				error();
			}
		}
		void constDefIntGroup() {
			switch (lookToken()) {
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				integer();
				constDefIntGroup();
				break;
			default:
				error();
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
				error();
			}
		}
		void constDefCharGroup() {
			switch (lookToken()) {
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::CHARCON);
				constDefCharGroup();
				break;
			default:
				error();
			}
		}
		void decHead() {
			switch (lookToken()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				eatToken(TokenType::IDENFR);
				break;
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				eatToken(TokenType::IDENFR);
				break;
			default:
				error();
			}
			syntaxOutput("<声明头部>");
		}
		void verDec() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				return;
				break;
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
					error();
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
					error();
				}
				break;
			case TokenType::VOIDTK:
				return;
				break;
			case TokenType::IFTK:
				return;
				break;
			case TokenType::DOTK:
				return;
				break;
			case TokenType::WHILETK:
				return;
				break;
			case TokenType::FORTK:
				return;
				break;
			case TokenType::SCANFTK:
				return;
				break;
			case TokenType::PRINTFTK:
				return;
				break;
			case TokenType::RETURNTK:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::LBRACE:
				return;
				break;
			case TokenType::RBRACE:
				return;
				break;
			default:
				error();
			}
			syntaxOutput("<变量说明>");
		}
		void verDef() {
			switch (lookToken()) {
			case TokenType::INTTK:
				typeIdent();
				verDefName();
				verDefNameGroup();
				break;
			case TokenType::CHARTK:
				typeIdent();
				verDefName();
				verDefNameGroup();
				break;
			default:
				error();
			}
			syntaxOutput("<变量定义>");
		}
		void verDefName() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				eatToken(TokenType::IDENFR);
				verDefNameArr();
				break;
			default:
				error();
			}
		}
		void verDefNameArr() {
			switch (lookToken()) {
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				return;
				break;
			case TokenType::LBRACK:
				eatToken(TokenType::LBRACK);
				uninteger();
				eatToken(TokenType::RBRACK);
				break;
			default:
				error();
			}
		}
		void verDefNameGroup() {
			switch (lookToken()) {
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				verDefName();
				verDefNameGroup();
				break;
			default:
				error();
			}
		}
		void typeIdent() {
			switch (lookToken()) {
			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				break;
			case TokenType::CHARTK:
				eatToken(TokenType::CHARTK);
				break;
			default:
				error();
			}
		}
		void retFuncDef() {
			switch (lookToken()) {
			case TokenType::INTTK:
				decHead();
				eatToken(TokenType::LPARENT);
				paraList();
				eatToken(TokenType::RPARENT);
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			case TokenType::CHARTK:
				decHead();
				eatToken(TokenType::LPARENT);
				paraList();
				eatToken(TokenType::RPARENT);
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				error();
			}
			syntaxOutput("<有返回值函数定义>");
		}
		void nonRetFuncDef() {
			switch (lookToken()) {
			case TokenType::VOIDTK:
				eatToken(TokenType::VOIDTK);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::LPARENT);
				paraList();
				eatToken(TokenType::RPARENT);
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				error();
			}
			syntaxOutput("<无返回值函数定义>");
		}
		void compStatement() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::CONSTTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::INTTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::CHARTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::IFTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::DOTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::WHILETK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::FORTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::SCANFTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::PRINTFTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::RETURNTK:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::SEMICN:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::LBRACE:
				constDec();
				verDec();
				statementCol();
				break;
			case TokenType::RBRACE:
				constDec();
				verDec();
				statementCol();
				break;
			default:
				error();
			}
			syntaxOutput("<复合语句>");
		}
		void paraList() {
			switch (lookToken()) {
			case TokenType::INTTK:
				typeIdent();
				eatToken(TokenType::IDENFR);
				paraListGroup();
				break;
			case TokenType::CHARTK:
				typeIdent();
				eatToken(TokenType::IDENFR);
				paraListGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				error();
			}
			syntaxOutput("<参数表>");
		}
		void paraListGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				typeIdent();
				eatToken(TokenType::IDENFR);
				paraListGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				error();
			}
		}
		void mainFunc() {
			switch (lookToken()) {
			case TokenType::VOIDTK:
				eatToken(TokenType::VOIDTK);
				eatToken(TokenType::MAINTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::RPARENT);
				eatToken(TokenType::LBRACE);
				compStatement();
				eatToken(TokenType::RBRACE);
				break;
			default:
				error();
			}
			syntaxOutput("<主函数>");
		}
		void exprPrefix() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				return;
				break;
			case TokenType::INTCON:
				return;
				break;
			case TokenType::CHARCON:
				return;
				break;
			case TokenType::PLUS:
				eatToken(TokenType::PLUS);
				break;
			case TokenType::MINU:
				eatToken(TokenType::MINU);
				break;
			case TokenType::LPARENT:
				return;
				break;
			default:
				error();
			}
		}
		void expr() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				exprPrefix();
				term();
				exprGroup();
				break;
			case TokenType::INTCON:
				exprPrefix();
				term();
				exprGroup();
				break;
			case TokenType::CHARCON:
				exprPrefix();
				term();
				exprGroup();
				break;
			case TokenType::PLUS:
				exprPrefix();
				term();
				exprGroup();
				break;
			case TokenType::MINU:
				exprPrefix();
				term();
				exprGroup();
				break;
			case TokenType::LPARENT:
				exprPrefix();
				term();
				exprGroup();
				break;
			default:
				error();
			}
			syntaxOutput("<表达式>");
		}
		void exprGroup() {
			switch (lookToken()) {
			case TokenType::PLUS:
				plusOp();
				term();
				exprGroup();
				break;
			case TokenType::MINU:
				plusOp();
				term();
				exprGroup();
				break;
			case TokenType::LSS:
				return;
				break;
			case TokenType::LEQ:
				return;
				break;
			case TokenType::GEQ:
				return;
				break;
			case TokenType::GRE:
				return;
				break;
			case TokenType::EQL:
				return;
				break;
			case TokenType::NEQ:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				return;
				break;
			case TokenType::RPARENT:
				return;
				break;
			case TokenType::RBRACK:
				return;
				break;
			default:
				error();
			}
		}
		void term() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				factor();
				termGroup();
				break;
			case TokenType::INTCON:
				factor();
				termGroup();
				break;
			case TokenType::CHARCON:
				factor();
				termGroup();
				break;
			case TokenType::PLUS:
				factor();
				termGroup();
				break;
			case TokenType::MINU:
				factor();
				termGroup();
				break;
			case TokenType::LPARENT:
				factor();
				termGroup();
				break;
			default:
				error();
			}
			syntaxOutput("<项>");
		}
		void termGroup() {
			switch (lookToken()) {
			case TokenType::PLUS:
				return;
				break;
			case TokenType::MINU:
				return;
				break;
			case TokenType::MULT:
				mulOp();
				factor();
				termGroup();
				break;
			case TokenType::DIV:
				mulOp();
				factor();
				termGroup();
				break;
			case TokenType::LSS:
				return;
				break;
			case TokenType::LEQ:
				return;
				break;
			case TokenType::GEQ:
				return;
				break;
			case TokenType::GRE:
				return;
				break;
			case TokenType::EQL:
				return;
				break;
			case TokenType::NEQ:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				return;
				break;
			case TokenType::RPARENT:
				return;
				break;
			case TokenType::RBRACK:
				return;
				break;
			default:
				error();
			}
		}
		void factorWithIdent() {
			switch (lookToken()) {
			case TokenType::PLUS:
				return;
				break;
			case TokenType::MINU:
				return;
				break;
			case TokenType::MULT:
				return;
				break;
			case TokenType::DIV:
				return;
				break;
			case TokenType::LSS:
				return;
				break;
			case TokenType::LEQ:
				return;
				break;
			case TokenType::GEQ:
				return;
				break;
			case TokenType::GRE:
				return;
				break;
			case TokenType::EQL:
				return;
				break;
			case TokenType::NEQ:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::COMMA:
				return;
				break;
			case TokenType::RPARENT:
				return;
				break;
			case TokenType::LBRACK:
				eatToken(TokenType::LBRACK);
				expr();
				eatToken(TokenType::RBRACK);
				break;
			case TokenType::RBRACK:
				return;
				break;
			default:
				error();
			}
		}
		void factor() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				switch (lookToken(1)) {
				case TokenType::LPARENT:
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
					eatToken(TokenType::IDENFR);
					factorWithIdent();
					break;
				default:
					error();
				}
				break;
			case TokenType::INTCON:
				integer();
				break;
			case TokenType::CHARCON:
				eatToken(TokenType::CHARCON);
				break;
			case TokenType::PLUS:
				integer();
				break;
			case TokenType::MINU:
				integer();
				break;
			case TokenType::LPARENT:
				eatToken(TokenType::LPARENT);
				expr();
				eatToken(TokenType::RPARENT);
				break;
			default:
				error();
			}
			syntaxOutput("<因子>");
		}
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
					error();
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
				eatToken(TokenType::RBRACE);
				break;
			default:
				error();
			}
			syntaxOutput("<语句>");
		}
		void assignWithIdent() {
			switch (lookToken()) {
			case TokenType::ASSIGN:
				eatToken(TokenType::ASSIGN);
				expr();
				break;
			case TokenType::LBRACK:
				eatToken(TokenType::LBRACK);
				expr();
				eatToken(TokenType::RBRACK);
				eatToken(TokenType::ASSIGN);
				expr();
				break;
			default:
				error();
			}
		}
		void assignStat() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				eatToken(TokenType::IDENFR);
				assignWithIdent();
				break;
			default:
				error();
			}
			syntaxOutput("<赋值语句>");
		}
		void condStat() {
			switch (lookToken()) {
			case TokenType::IFTK:
				eatToken(TokenType::IFTK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				statement();
				condStatElse();
				break;
			default:
				error();
			}
			syntaxOutput("<条件语句>");
		}
		void condStatElse() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				return;
				break;
			case TokenType::IFTK:
				return;
				break;
			case TokenType::ELSETK:
				eatToken(TokenType::ELSETK);
				statement();
				break;
			case TokenType::DOTK:
				return;
				break;
			case TokenType::WHILETK:
				return;
				break;
			case TokenType::FORTK:
				return;
				break;
			case TokenType::SCANFTK:
				return;
				break;
			case TokenType::PRINTFTK:
				return;
				break;
			case TokenType::RETURNTK:
				return;
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::LBRACE:
				return;
				break;
			case TokenType::RBRACE:
				return;
				break;
			default:
				error();
			}
		}
		void cond() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				expr();
				condExprEnd();
				break;
			case TokenType::INTCON:
				expr();
				condExprEnd();
				break;
			case TokenType::CHARCON:
				expr();
				condExprEnd();
				break;
			case TokenType::PLUS:
				expr();
				condExprEnd();
				break;
			case TokenType::MINU:
				expr();
				condExprEnd();
				break;
			case TokenType::LPARENT:
				expr();
				condExprEnd();
				break;
			default:
				error();
			}
			syntaxOutput("<条件>");
		}
		void condExprEnd() {
			switch (lookToken()) {
			case TokenType::LSS:
				relationOp();
				expr();
				break;
			case TokenType::LEQ:
				relationOp();
				expr();
				break;
			case TokenType::GEQ:
				relationOp();
				expr();
				break;
			case TokenType::GRE:
				relationOp();
				expr();
				break;
			case TokenType::EQL:
				relationOp();
				expr();
				break;
			case TokenType::NEQ:
				relationOp();
				expr();
				break;
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				error();
			}
		}
		void cycleStat() {
			switch (lookToken()) {
			case TokenType::DOTK:
				eatToken(TokenType::DOTK);
				statement();
				eatToken(TokenType::WHILETK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::WHILETK:
				eatToken(TokenType::WHILETK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				statement();
				break;
			case TokenType::FORTK:
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
				error();
			}
			syntaxOutput("<循环语句>");
		}
		void step() {
			switch (lookToken()) {
			case TokenType::INTCON:
				uninteger();
				break;
			default:
				error();
			}
			syntaxOutput("<步长>");
		}
		void funcCall() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::LPARENT);
				valParaList();
				eatToken(TokenType::RPARENT);
				break;
			default:
				error();
			}
			// TODO(zyc): 强行有返回值
			syntaxOutput("<有返回值函数调用语句>");
		}
		void valParaList() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				expr();
				valParaListGroup();
				break;
			case TokenType::INTCON:
				expr();
				valParaListGroup();
				break;
			case TokenType::CHARCON:
				expr();
				valParaListGroup();
				break;
			case TokenType::PLUS:
				expr();
				valParaListGroup();
				break;
			case TokenType::MINU:
				expr();
				valParaListGroup();
				break;
			case TokenType::LPARENT:
				expr();
				valParaListGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				error();
			}
			syntaxOutput("<值参数表>");
		}
		void valParaListGroup() {
			switch (lookToken()) {
			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				expr();
				valParaListGroup();
				break;
			case TokenType::RPARENT:
				return;
				break;
			default:
				error();
			}
		}
		void statementCol() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				statement();
				statementCol();
				break;
			case TokenType::IFTK:
				statement();
				statementCol();
				break;
			case TokenType::DOTK:
				statement();
				statementCol();
				break;
			case TokenType::WHILETK:
				statement();
				statementCol();
				break;
			case TokenType::FORTK:
				statement();
				statementCol();
				break;
			case TokenType::SCANFTK:
				statement();
				statementCol();
				break;
			case TokenType::PRINTFTK:
				statement();
				statementCol();
				break;
			case TokenType::RETURNTK:
				statement();
				statementCol();
				break;
			case TokenType::SEMICN:
				statement();
				statementCol();
				break;
			case TokenType::LBRACE:
				statement();
				statementCol();
				break;
			case TokenType::RBRACE:
				return;
				break;
			default:
				error();
			}
			syntaxOutput("<语句列>");
		}
		void scanfStat() {
			switch (lookToken()) {
			case TokenType::SCANFTK:
				eatToken(TokenType::SCANFTK);
				eatToken(TokenType::LPARENT);
				eatToken(TokenType::IDENFR);
				identGroup();
				eatToken(TokenType::RPARENT);
				break;
			default:
				error();
			}
			syntaxOutput("<读语句>");
		}
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
				error();
			}
		}
		void printfStat() {
			switch (lookToken()) {
			case TokenType::PRINTFTK:
				eatToken(TokenType::PRINTFTK);
				eatToken(TokenType::LPARENT);
				printfStatEnd();
				break;
			default:
				error();
			}
			syntaxOutput("<写语句>");
		}
		void printfStatEnd() {
			switch (lookToken()) {
			case TokenType::IDENFR:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::INTCON:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::CHARCON:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::STRCON:
				strconst();
				printfStr();
				break;
			case TokenType::PLUS:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::MINU:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			case TokenType::LPARENT:
				expr();
				eatToken(TokenType::RPARENT);
				break;
			default:
				error();
			}
		}
		void printfStr() {
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
				error();
			}
		}
		void retStat() {
			switch (lookToken()) {
			case TokenType::RETURNTK:
				eatToken(TokenType::RETURNTK);
				retStatEnd();
				break;
			default:
				error();
			}
			syntaxOutput("<返回语句>");
		}
		void retStatEnd() {
			switch (lookToken()) {
			case TokenType::SEMICN:
				return;
				break;
			case TokenType::LPARENT:
				eatToken(TokenType::LPARENT);
				expr();
				eatToken(TokenType::RPARENT);
				break;
			default:
				error();
			}
		}

#pragma endregion 
		
		TokenType::Type lookToken(int ahead = 0) {
			return lex_parser_.lookToken(ahead).unwrap().getTokenType().type_;
		}

		void eatToken(TokenType excepted_token_type) {
			Token token_type = lex_parser_.eatToken().unwrap();
			if (token_type.getTokenType().type_ != excepted_token_type.type_) {
				panic("unimplemented");
			}
		}

		void error() {
			panic("unimplemented");
		}
		
		lex::LexParser& lex_parser_;

	};

}
}

#endif
