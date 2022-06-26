#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>
#include "AST.h"
#include "../lexer/lexer.h"

class Parser {
    private:
        int token;
        Lexer* lexer;

        void nextToken();
        ExpressionAST* parseNumberExpression();
        ExpressionAST* parseStringExpression();
        ExpressionAST* parseParenthesisExpression();
        ExpressionAST* parseIdentifierExpression();
        ExpressionAST* parseExpression();
        ExpressionAST* parsePrimary();
        ExpressionAST* parseBinOpRHS(int exprPrec, ExpressionAST* LHS);
        PrototypeAST* parsePrototype();
        VariableDeclarationAST* parseVariableDeclaration();
    public:
        Parser(Lexer* lexer);
        void loop();
};

#endif