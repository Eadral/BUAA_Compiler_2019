//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_TOKEN_HPP
#define BUAAC_TOKEN_HPP

#include <string>

namespace lex {

    enum TokenType{
        ID,
        INT_TYPE,
        REAL_TYPE,
        NUM,
        CHAR_TYPE,
        CHAR,
        STRING_TYPE,
        STRING,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        IF,
        WHILE,
        FOR,
        DO,
        COMMA,
        RETURN,
        SEMI,
        END,
        ASSIGN,
    };

    class Token {
    public:


        explicit Token(TokenType token_type) {
            token_type_ = token_type;
        }

        Token(TokenType token_type, std::string& value) {
            token_type_ = token_type;
            value_ = value;
        }

        TokenType getTokenType() const {
            return token_type_;
        }

        const std::string &getValue() const {
            return value_;
        }

    private:
        TokenType token_type_;
        std::string value_;
    };
}



#endif //BUAAC_TOKEN_HPP
