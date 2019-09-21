//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_TOKEN_HPP
#define BUAAC_TOKEN_HPP

#include <string>
#include "../meow/core.hpp"
#include <iostream>

#include <regex>

namespace buaac {
	namespace lex {
		#pragma region TokenType
		ENUM_START(TokenType)
			IDENFR,
			INTCON,
			CHARCON,
			STRCON,

			CONSTTK,
			INTTK,
			CHARTK,
			VOIDTK,
			MAINTK,
			IFTK,
			ELSETK,
			DOTK,
			WHILETK,
			FORTK,
			SCANFTK,
			PRINTFTK,
			RETURNTK,

			PLUS,
			MINU,
			MULT,
			DIV,
			LSS,
			LEQ,
			GRE,
			GEQ,
			EQL,
			NEQ,
			ASSIGN,
			SEMICN,
			COMMA,
			LPARENT,
			RPARENT,
			LBRACK,
			RBARCK,
			LBRACE,
			RBRACE,
		ENUM_MED(TokenType)
		ENUM_DEFAULT_OUTPUT(IDENFR)
		ENUM_DEFAULT_OUTPUT(INTCON)
		ENUM_DEFAULT_OUTPUT(CHARCON)
		ENUM_DEFAULT_OUTPUT(STRCON)
		ENUM_DEFAULT_OUTPUT(CONSTTK)
		ENUM_DEFAULT_OUTPUT(INTTK)
		ENUM_DEFAULT_OUTPUT(CHARTK)
		ENUM_DEFAULT_OUTPUT(VOIDTK)
		ENUM_DEFAULT_OUTPUT(MAINTK)
		ENUM_DEFAULT_OUTPUT(IFTK)
		ENUM_DEFAULT_OUTPUT(ELSETK)
		ENUM_DEFAULT_OUTPUT(DOTK)
		ENUM_DEFAULT_OUTPUT(WHILETK)
		ENUM_DEFAULT_OUTPUT(FORTK)
		ENUM_DEFAULT_OUTPUT(SCANFTK)
		ENUM_DEFAULT_OUTPUT(PRINTFTK)
		ENUM_DEFAULT_OUTPUT(RETURNTK)
		ENUM_DEFAULT_OUTPUT(PLUS)
		ENUM_DEFAULT_OUTPUT(MINU)
		ENUM_DEFAULT_OUTPUT(MULT)
		ENUM_DEFAULT_OUTPUT(DIV)
		ENUM_DEFAULT_OUTPUT(LSS)
		ENUM_DEFAULT_OUTPUT(LEQ)
		ENUM_DEFAULT_OUTPUT(GRE)
		ENUM_DEFAULT_OUTPUT(GEQ)
		ENUM_DEFAULT_OUTPUT(EQL)
		ENUM_DEFAULT_OUTPUT(NEQ)
		ENUM_DEFAULT_OUTPUT(ASSIGN)
		ENUM_DEFAULT_OUTPUT(SEMICN)
		ENUM_DEFAULT_OUTPUT(COMMA)
		ENUM_DEFAULT_OUTPUT(LPARENT)
		ENUM_DEFAULT_OUTPUT(RPARENT)
		ENUM_DEFAULT_OUTPUT(LBRACK)
		ENUM_DEFAULT_OUTPUT(RBARCK)
		ENUM_DEFAULT_OUTPUT(LBRACE)
		ENUM_DEFAULT_OUTPUT(RBRACE)
		ENUM_END()
#pragma endregion 
			
		class Token: implement fmt::Display {
		public:

			Token() = default;

			Token(TokenType token_type) {
				token_type_ = token_type;
				value_ = "";
			}

			Token(TokenType token_type, std::string& value) {
				token_type_ = token_type;
				value_ = value;
			}

			Token(const Token& other)
				: token_type_(other.token_type_),
				  value_(other.value_),
				  has_value_(other.has_value_) {
			}

			Token(Token&& other) noexcept
				: token_type_(other.token_type_),
				  value_(std::move(other.value_)),
				  has_value_(other.has_value_) {
			}

			Token& operator=(const Token& other) {
				if (this == &other)
					return *this;
				token_type_ = other.token_type_;
				value_ = other.value_;
				has_value_ = other.has_value_;
				return *this;
			}

			Token& operator=(Token&& other) noexcept {
				if (this == &other)
					return *this;
				token_type_ = other.token_type_;
				value_ = std::move(other.value_);
				has_value_ = other.has_value_;
				return *this;
			}

			TokenType getTokenType() const {
				return token_type_;
			}

			const std::string& getValue() const {
				return value_;
			}

			std::string toString() {
				return fmt::Format::toString(token_type_);
			}
		private:
			TokenType token_type_;
			std::string value_;
			bool has_value_;
		};


		ENUM_START(LexError)
			UNEXPECTED,
		ENUM_MED(LexError)
			ENUM_DEFAULT_OUTPUT(UNEXPECTED)
		ENUM_END()

		using LexResult = Result<Token, LexError>;

		LexResult eat_by_regex(std::string& in) {
			std::smatch result;
			
			if (std::regex_search(in, result, std::regex("^if"))) {
				return LexResult::Ok(Token(TokenType::IFTK));
			}

			
			if (std::regex_search(in, result, std::regex("^return"))) {
				return LexResult::Ok(Token(TokenType::RETURNTK));
			}

			return LexResult::Err(LexError::UNEXPECTED);
		}
		
		LexResult eat(std::string& in) {
			return eat_by_regex(in);
		}

	}

}
#endif //BUAAC_TOKEN_HPP

