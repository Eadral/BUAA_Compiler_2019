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

#pragma region Gererated

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
				panic("{'funcDef funcDefGroup', 'EMPTY'}");
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

			case TokenType::LBRACE:
				return;

				break;

			default:
				error();
			}
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
		}

		void constDefInt() {
			switch (lookToken()) {

			case TokenType::INTTK:
				eatToken(TokenType::INTTK);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::INTCON);


				break;

			default:
				error();
			}
		}

		void constDefIntGroup() {
			switch (lookToken()) {

			case TokenType::CONSTTK:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::COMMA:
				eatToken(TokenType::COMMA);
				eatToken(TokenType::IDENFR);
				eatToken(TokenType::ASSIGN);
				eatToken(TokenType::INTCON);
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

			case TokenType::CONSTTK:
				return;

				break;

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
		}

		void verDec() {
			switch (lookToken()) {

			case TokenType::INTTK:
				panic("{'verDef SEMICN verDec', 'EMPTY'}");
				break;

			case TokenType::CHARTK:
				return;

				break;

			case TokenType::VOIDTK:
				return;

				break;

			case TokenType::LBRACE:
				return;

				break;

			default:
				error();
			}
		}

		void verDef() {
			switch (lookToken()) {

			case TokenType::INTTK:
				typeIdent();
				verDefName();
				verDefNameGroup();


				break;

			default:
				error();
			}
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

			case TokenType::INTTK:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::COMMA:
				return;

				break;

			case TokenType::LBRACK:
				eatToken(TokenType::LBRACK);
				eatToken(TokenType::INTCON);
				eatToken(TokenType::RBRACK);


				break;

			default:
				error();
			}
		}

		void verDefNameGroup() {
			switch (lookToken()) {

			case TokenType::INTTK:
				return;

				break;

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
		}

		void compStatement() {
			switch (lookToken()) {

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

			case TokenType::LBRACE:
				constDec();
				verDec();
				statementCol();


				break;

			default:
				error();
			}
		}

		void paraList() {
			switch (lookToken()) {

			case TokenType::INTTK:
				typeIdent();
				eatToken(TokenType::IDENFR);
				paraListGroup();


				break;

			case TokenType::RPARENT:
				return;

				break;

			case TokenType::LBRACE:
				return;

				break;

			default:
				error();
			}
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

			case TokenType::LBRACE:
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
				panic("{'PLUS', 'EMPTY'}");
				break;

			case TokenType::MINU:
				panic("{'MINU', 'EMPTY'}");
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
		}

		void exprGroup() {
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

			case TokenType::IFTK:
				return;

				break;

			case TokenType::ELSETK:
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

			case TokenType::PLUS:
				panic("{'plusOp term exprGroup', 'EMPTY'}");
				break;

			case TokenType::MINU:
				panic("{'plusOp term exprGroup', 'EMPTY'}");
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

			case TokenType::ASSIGN:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::COMMA:
				return;

				break;

			case TokenType::LPARENT:
				return;

				break;

			case TokenType::RPARENT:
				return;

				break;

			case TokenType::LBRACK:
				return;

				break;

			case TokenType::RBRACK:
				return;

				break;

			case TokenType::LBRACE:
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

			case TokenType::LPARENT:
				factor();
				termGroup();


				break;

			default:
				error();
			}
		}

		void termGroup() {
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

			case TokenType::IFTK:
				return;

				break;

			case TokenType::ELSETK:
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

			case TokenType::ASSIGN:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::COMMA:
				return;

				break;

			case TokenType::LPARENT:
				return;

				break;

			case TokenType::RPARENT:
				return;

				break;

			case TokenType::LBRACK:
				return;

				break;

			case TokenType::RBRACK:
				return;

				break;

			case TokenType::LBRACE:
				return;

				break;

			default:
				error();
			}
		}

		void factorWithIdent() {
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

			case TokenType::IFTK:
				return;

				break;

			case TokenType::ELSETK:
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

			case TokenType::ASSIGN:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::COMMA:
				return;

				break;

			case TokenType::LPARENT:
				return;

				break;

			case TokenType::RPARENT:
				return;

				break;

			case TokenType::LBRACK:
				panic("{'LBRACK expr RBRACK', 'EMPTY'}");
				break;

			case TokenType::RBRACK:
				return;

				break;

			case TokenType::LBRACE:
				return;

				break;

			default:
				error();
			}
		}

		void factor() {
			switch (lookToken()) {

			case TokenType::IDENFR:
				panic("{'funcCall', 'IDENFR factorWithIdent'}");
				break;

			case TokenType::INTCON:
				eatToken(TokenType::INTCON);


				break;

			case TokenType::CHARCON:
				eatToken(TokenType::CHARCON);


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

		void statement() {
			switch (lookToken()) {

			case TokenType::IDENFR:
				panic("{'assignStat SEMICN', 'funcCall SEMICN'}");
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

			case TokenType::ASSIGN:
				assignStat();
				eatToken(TokenType::SEMICN);


				break;

			case TokenType::SEMICN:
				eatToken(TokenType::SEMICN);


				break;

			case TokenType::LBRACK:
				assignStat();
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


				break;

			case TokenType::ASSIGN:
				assignWithIdent();


				break;

			case TokenType::LBRACK:
				assignWithIdent();


				break;

			default:
				error();
			}
		}

		void condStat() {
			switch (lookToken()) {

			case TokenType::IFTK:
				eatToken(TokenType::IFTK);
				eatToken(TokenType::LPARENT);
				cond();
				eatToken(TokenType::RPARENT);
				condStatElse();


				break;

			default:
				error();
			}
		}

		void condStatElse() {
			switch (lookToken()) {

			case TokenType::ELSETK:
				eatToken(TokenType::ELSETK);
				statement();


				break;

			case TokenType::WHILETK:
				return;

				break;

			case TokenType::LPARENT:
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
		}

		void condExprEnd() {
			switch (lookToken()) {

			case TokenType::IDENFR:
				return;

				break;

			case TokenType::IFTK:
				return;

				break;

			case TokenType::ELSETK:
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

			case TokenType::ASSIGN:
				return;

				break;

			case TokenType::SEMICN:
				return;

				break;

			case TokenType::RPARENT:
				return;

				break;

			case TokenType::LBRACK:
				return;

				break;

			case TokenType::LBRACE:
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
				eatToken(TokenType::INTCON);
				eatToken(TokenType::RPARENT);
				statement();


				break;

			default:
				error();
			}
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

			case TokenType::LBRACE:
				eatToken(TokenType::LBRACE);
				statement();
				eatToken(TokenType::RBRACE);


				break;

			default:
				error();
			}
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
				eatToken(TokenType::STRCON);
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

#pragma region Gererated

#pragma endregion 
		
		TokenType::Type lookToken(int ahead = 0) {
			return lex_parser_.lookToken().unwrap().getTokenType().type_;
		}

		void eatToken(TokenType excepted_token_type) {
			auto token_type = lex_parser_.eatToken();
		}

		void error() {
			panic("unimplemented");
		}
		
		lex::LexParser& lex_parser_;

	};

}
}

#endif
