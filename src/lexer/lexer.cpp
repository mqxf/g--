#include <string>
#include <cctype>
#include "lexer.h"
#include "../utils/utils.h"

Lexer::Lexer(std::string src) {
    this->src = src;
    length = this->src.length();
    i = 0;
    c = src[0];
}

char Lexer::peek(size_t offset) {
    if (i + offset < length) {
        return src[i + offset];
    }
    return 0;
} 

void Lexer::advance() {
    if ((i + 1) < length) {
        i++;
        c = src[i];
    }
    else {
        i++;
        c = 0;
    }
}

void Lexer::skipWhiteSpace() {
    while (c == 10 || c == 13 || c == '\t' || c == ' ') {
        advance();
    }
}

void Lexer::skipComment() {
    if (c == '.' && peek(1) == '.') {
        if (peek(2) == '.') {
            advance();
            for (;;) {
                if (c == '.' && peek(1) == '.' && peek(2) == '.') {
                    advance();
                    advance();
                    advance();
                    break;
                }
                advance();
            }
        }
        else {
            while (c !='\n') advance();
        }
        skipWhiteSpace();
        skipComment();
    }
}

int Lexer::nextToken() {
    skipWhiteSpace();
    skipComment();

    if (this->c == EOF) return TKN_EOF;

    if (isalpha(this->c)) {
        this->str = this->c;
        advance();
        while (isalnum(this->c) || this->c == '_') {
            this->str.push_back(this->c);
            advance();
        }

        if (this->str == "int") return TKN_INT;
        if(this->str == "float") return TKN_FLOAT;
        if (this->str == "long") return TKN_LONG;
        if (this->str == "char") return TKN_CHAR;
        if (this->str == "const") return TKN_CONST;
        if (this->str == "if") return TKN_IF;
        if (this->str == "else") return TKN_ELSE;
        if (this->str == "switch") return TKN_SWITCH;
        if (this->str == "return") return TKN_RETURN;
        if (this->str == "break") return TKN_BREAK;
        if (this->str == "continue") return TKN_CONTINUE;
        if (this->str == "while") return TKN_WHILE;
        if (this->str == "extern") return TKN_EXTERN;
        return TKN_IDENTIFIER;
    }    
    
    if(isdigit(this->c)) {
        bool fl = false;
        std::string val;
        val.push_back(this->c);
        advance();

        while (isdigit(this->c) || this->c == '.') {
            if (this->c == '.') fl = true;
            val.push_back(this->c);
            advance();
        }

        if (fl) {
            this->dVal = strtod(val.c_str(), 0);
            this->lastFloat = true;
            return TKN_FLOAT_LITERAL;
        }
        else {
            this->lVal = strtoll(val.c_str(), 0, 10);
            this->lastFloat = false;
            return TKN_INT_LITERAL;
        }
    }

    if (this->c == '"') {
        std::string value;
        advance();
        while (c != '"') {
            value.push_back(c);
            advance();
        }
        advance();
        this->str = strFormat(value);
        return TKN_STRING_LITERAL;
    }

    if (this->c == '\'') {
        std::string value;
        advance();
        while (c != '\'') {
            value.push_back(c);
            advance();
        }
        advance();
        this->cVal = strFormat(value).at(0);
        return TKN_CHAR_LITERAL;
    }

    int ret = this->c;
    advance();
    return ret;    
}
