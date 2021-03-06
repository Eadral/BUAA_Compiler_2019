#ifndef BUAAC_LEXPARSE_HPP
#define BUAAC_LEXPARSE_HPP


#include "../meow/core.hpp"
#include "token.hpp"
#include <ctype.h>
#include <sstream>
#include <string>


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

#define simpleMatch(regexStr, tokenType)	\
	do	{	\
		string value = string(regexStr);	\
		if (meow::starts_with(in, string(regexStr))) {	\
				in = in.substr(string(regexStr).length());	\
				return LexResult::Ok(Token(TokenType::tokenType, value));	\
		}	\
	} while (0)

#define identMatch(valueStr, tokenType)	\
	do   \
		if (value == valueStr) {	\
			return LexResult::Ok(Token(TokenType::tokenType, value));	\
		}	\
	while(0)
		
		using LexResult = Result<Token, LexError>;

		class LexParser {
		public:

			bool hasError() {
				return has_error_;
			}
			
			LexParser(string source, string output) : source_(source){
				pointer_ = source_;
				if (output.find('v') != string::npos) {
					output_lex_ = true;
				}
				if (output.find('e') != string::npos) {
					output_error_ = true;
				}
			}

			LexResult eatToken() {
				LexResult result = eat(pointer_, true);
				auto token = result.unwrap();
				if (output_lex_) std::cout << token << std::endl;
				return result;
			}
			
			LexResult lookToken(int ahead = 0) {
				string look_ = pointer_;
				while (ahead--) {
					eat(look_);
				}
				return eat(look_);
			}

			int getLineNumber() const {
				return line_;
			}

			bool LastIsLine() {
				return last_is_line_;
			}
			
		private:
			const string source_;
			string pointer_;
			bool output_lex_ = false;
			bool output_error_ = false;
			int line_ = 1;
			int col = 1;

			bool last_is_line_ = false;

			bool has_error_ = false;
			
			bool isString(char c) {
				return (c >= 35 && c <= 126) || c == 32 || c == 33;
			}

			bool isAlpha(char c) {
				return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
			}

			bool isPlus(char c) {
				return c == '+' || c == '-';
			}

			bool isMul(char c) {
				return c == '*' || c == '/';
			}

			bool isDigit(char c) {
				return c >= '0' && c <= '9';
			}
			
			bool isChar(char c) {
				return isPlus(c) || isMul(c) || isAlpha(c) || isDigit(c);
			}

			LexResult eat(string& in, bool advance = false) {
				if (in.length() == 0) {
					return LexResult::Err(LexError::END);
				}
				
				auto begin = in.cbegin();
				auto end = in.cend();

				if (last_is_line_)
					last_is_line_ = false;
				
				while (begin < end && isspace(*begin)) {
					if (*begin == '\n' && advance) {
						line_++;
						col = 1;
						last_is_line_ = true;
					}
					++begin;
				}
					
				in = in.substr(begin - in.cbegin());
				begin = in.cbegin();
				end = in.cend();


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


				// TODO(zyc): Error Handle

				if (isAlpha(in[0])) {
					std::istringstream sin(in);
					std::ostringstream buf;
					char c;

					while (c = sin.get(), isAlpha(c) || isDigit(c)) {
						buf << c;
					}

					string value = buf.str();
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

				if (isDigit(in[0])) {
					std::istringstream sin(in);
					std::ostringstream buf;
					char c;

					while (c = sin.get(), isDigit(c)) {
						buf << c;
					}
					string value = buf.str();
					in = in.substr(value.length());
					return LexResult::Ok(Token(TokenType::INTCON, value));
				}

				// �ַ�
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


					string value = buf.str();
					if (!isChar(value[0])) {
						if (advance) error();
					}
					in = in.substr(value.length() + 2);
					return LexResult::Ok(Token(TokenType::CHARCON, value));
				}

				// �ַ���
				if (in[0] == '\"') {
					std::istringstream sin(in);
					std::ostringstream buf;
					char c;

					c = sin.get();

					int offset = 0;
					
					while (c = sin.get(), isString(c) && c != '\"') {
						buf << c;
						if (c == '\\') {
							buf << c;
							offset--;
						}
					}

					if (c != '\"')
						c = sin.get();

					if (c != '\"') {
						// debugln("{}\n", c);
						panic("unimplemented");
					}


					string value = buf.str();
					in = in.substr(value.length() + 2 + offset);
					return LexResult::Ok(Token(TokenType::STRCON, value));
				}

				if (advance) error();
				return LexResult::Err(LexError::UNEXPECTED);
			}

			void error(char code = 'a') {
				has_error_ = true;
				if (output_error_) std::cout << getLineNumber() << " " << code << std::endl;
				// panic("aho");
			}

		};

	}
}


#endif
