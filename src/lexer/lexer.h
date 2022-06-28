#ifndef _LEXER_H
#define _LEXER_H

#include <string>

enum Token {

    //All basic C-- keywords

    //control
    TKN_EOF = -1,   
    
    //keywords
    TKN_INT = -2,
    TKN_FLOAT = -3,
    TKN_LONG = -4,
    TKN_CHAR = -5,
    TKN_CONST = -6,
    TKN_IF = -7,
    TKN_ELSE = -8,
    TKN_SWITCH = -9,
    TKN_RETURN = -10,
    TKN_BREAK = -11,
    TKN_CONTINUE = -12,
    TKN_WHILE = -13,
    TKN_EXTERN = -14,
    TKN_VOID = -15,

    //literals
    TKN_IDENTIFIER = -16,
    TKN_FLOAT_LITERAL = -17,
    TKN_INT_LITERAL = -18,
    TKN_STRING_LITERAL = -19,
    TKN_CHAR_LITERAL = -20,
};

class Lexer {
    private:
        std::string src;
        size_t length;
        size_t i;
        char c;

        std::string str;
        char cVal;
        double dVal;
        long long lVal;
        bool lastFloat;

        void skipWhiteSpace();
        void skipComment();
        void advance();

    public:
        Lexer(std::string src);
        int nextToken();
        char peek(size_t offset);
        std::string getStr() { return str; }
        char getCVal() { return cVal; }
        double getDVal() { return dVal; }
        long long getLVal() { return lVal; }
        bool isLastFloat() { return lastFloat; }
};

#endif