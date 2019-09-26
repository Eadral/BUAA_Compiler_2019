#ifndef BUAAC_LEXPARSE_HPP
#define BUAAC_LEXPARSE_HPP


#include "../meow/core.hpp"
#include "token.hpp"
#include <ctype.h>
#include <sstream>
#include <string>

namespace std {
	template <typename charT>
	inline bool starts_with(const basic_string<charT>& big, const basic_string<charT>& small) {
		if (&big == &small) return true;
		const typename basic_string<charT>::size_type big_size = big.size();
		const typename basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool starts_with_ = (big.compare(0, small_size, small) == 0);
		return valid_ && starts_with_;
	}

	template <typename charT>
	inline bool ends_with(const basic_string<charT>& big, const basic_string<charT>& small) {
		if (&big == &small) return true;
		const typename basic_string<charT>::size_type big_size = big.size();
		const typename basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool ends_with_ = (big.compare(big_size - small_size, small_size, small) == 0);
		return valid_ && ends_with_;
	}
}  // namespace std

namespace buaac {
	namespace lex {

		#pragma region LexError
		ENUM_START(LexError)
			UNEXPECTED,
			END,
			ENUM_MED(LexError)
			ENUM_DEFAULT_OUTPUT(UNEXPECTED)
			ENUM_DEFAULT_OUTPUT(END)
		ENUM_END()
		#pragma endregion

		using LexResult = Result<Token, LexError>;

		class LexParser {
			LexParser(std::string source) :source_(source) {}

			

		private:
			std::string source_;

			
			
		};
		
		LexResult eat_machine(std::string& in) {
			std::ostringstream out;
			panic("unimplemented");
		}

#define simpleMatch(regexStr, tokenType)	\
	do	{	\
		std::string value = std::string(regexStr);	\
		if (std::starts_with(in, std::string(regexStr))) {	\
				in = in.substr(std::string(regexStr).length());	\
				return LexResult::Ok(Token(TokenType::tokenType, value));	\
		}	\
	} while (0)

#define identMatch(valueStr, tokenType)	\
	do   \
		if (value == valueStr) {	\
			return LexResult::Ok(Token(TokenType::tokenType, value));	\
		}	\
	while(0)
		
		LexResult eat_by_regex(std::string& in) {
			std::smatch result;

			auto begin = in.cbegin();
			auto end = in.cend();

			while (begin < end && isspace(*begin))
				++begin;
			
			in = in.substr(begin - in.cbegin());
			begin = in.cbegin();
			end = in.cend();
			
			// simpleMatch("const", CONSTTK);
			// simpleMatch("int", INTTK);
			// simpleMatch("char", CHARTK);
			// simpleMatch("void", VOIDTK);
			// simpleMatch("main", MAINTK);
			// simpleMatch("if", IFTK);
			// simpleMatch("else", ELSETK);
			// simpleMatch("do", DOTK);
			// simpleMatch("while", WHILETK);
			// simpleMatch("for", FORTK);
			// simpleMatch("scanf", SCANFTK);
			// simpleMatch("printf", PRINTFTK);
			// simpleMatch("return", RETURNTK);
			
			simpleMatch("+", PLUS);
			simpleMatch("-", MINU);
			simpleMatch("*", MULT);
			simpleMatch("/", DIV);
			simpleMatch("<=", LEQ);
			simpleMatch("<", LSS);
			simpleMatch(">=", GEQ);
			simpleMatch(">", GRE);
			simpleMatch("==", EQL);
			simpleMatch("!=", NEQ);
			simpleMatch("=", ASSIGN);
			simpleMatch(";", SEMICN);
			simpleMatch(",", COMMA);
			simpleMatch("(", LPARENT);
			simpleMatch(")", RPARENT);
			simpleMatch("[", LBRACK);
			simpleMatch("]", RBRACK);
			simpleMatch("{", LBRACE);
			simpleMatch("}", RBRACE);


			if (in.length() == 0) {
				return LexResult::Err(LexError::END);
			}

			if (isalpha(in[0]) || in[0] == '_') {
				std::istringstream sin(in);
				std::ostringstream buf;
				char c;

				while (c = sin.get(), isalpha(c) || isdigit(c) || c == '_' ) {
					buf << c;
				}
				
				std::string value = buf.str();
				in = in.substr(value.length());

				identMatch("const", CONSTTK);
				identMatch("int", INTTK);
				identMatch("char", CHARTK);
				identMatch("void", VOIDTK);
				identMatch("main", MAINTK);
				identMatch("if", IFTK);
				identMatch("else", ELSETK);
				identMatch("do", DOTK);
				identMatch("while", WHILETK);
				identMatch("for", FORTK);
				identMatch("scanf", SCANFTK);
				identMatch("printf", PRINTFTK);
				identMatch("return", RETURNTK);
				
				
				return LexResult::Ok(Token(TokenType::IDENFR, value));
			}

			if (isdigit(in[0])) {
				std::istringstream sin(in);
				std::ostringstream buf;
				char c;

				while (c = sin.get(), isdigit(c)) {
					buf << c;
				}
				std::string value = buf.str();
				in = in.substr(value.length());
				return LexResult::Ok(Token(TokenType::INTCON, value));
			}

			if (in[0] == '\'') {
				std::istringstream sin(in);
				std::ostringstream buf;
				char c;

				c = sin.get();
				c = sin.get();
				buf << c;
				c = sin.get();

				if (c != '\'') {
					panic("unimplemented");
				}
				
				
				
				std::string value = buf.str();
				in = in.substr(value.length()+2);
				return LexResult::Ok(Token(TokenType::CHARCON, value));
			}

			if (in[0] == '\"') {
				std::istringstream sin(in);
				std::ostringstream buf;
				char c;

				c = sin.get();

				while (c = sin.get(), ( (c >= 35 && c <= 126) || c == 32 || c == 33  ) && c != '\"' ) {
					buf << c;
				}

				if (c != '\"')
					c = sin.get();

				if (c != '\"') {
					debugln("{}\n", c);
					panic("unimplemented");
				}



				std::string value = buf.str();
				in = in.substr(value.length()+2);
				return LexResult::Ok(Token(TokenType::STRCON, value));
			}
			
			
			return LexResult::Err(LexError::UNEXPECTED);
		}

		LexResult eat(std::string& in) {
			return eat_by_regex(in);
		}
		
	}
}


#endif