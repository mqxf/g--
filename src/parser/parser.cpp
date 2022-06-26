#include <string>
#include <vector>
#include "parser.h"
#include "AST.h"
#include "../lexer/lexer.h"
#include "../utils/utils.h"

Parser::Parser(Lexer *lexer) {
    this->lexer = lexer;
    this->token = lexer->nextToken();
}

void Parser::nextToken() {
    this->token = lexer->nextToken();
}

ExpressionAST* Parser::parseNumberExpression() {    
    NumberExpressionAST* res = new NumberExpressionAST(this->lexer->isLastFloat() ? this->lexer->getDVal() : this->lexer->getLVal());
    this->nextToken();
    return res;
}

ExpressionAST* Parser::parseStringExpression() {
    StringExpressionAST* res = new StringExpressionAST(this->lexer->getStr());
    this->nextToken();
    return res;
}

ExpressionAST* Parser::parseParenthesisExpression() {
    this->nextToken();
    auto val = parseExpression();
    if (!val) return nullptr;

    if (this->token != ')') {
        log("Expected '('", Severity::ERROR);
        return nullptr;
    }

    this->nextToken();
    return val;
}

ExpressionAST* Parser::parseIdentifierExpression() {
    std::string name = this->lexer->getStr();

    this->nextToken();

    if (this->token != '(') {
        return new VariableExpressionAST(name);
    }

    this->nextToken();
    std::vector<ExpressionAST*> args;
    if (this->token != ')') {
        while (1) {
            if (auto arg = this->parseExpression()) {
                args.push_back(std::move(arg));
            }
            else {
                return nullptr;
            }
            
            if (this->token == ')') {
                break;
            }
            
            if (this->token != ',') {
                log("Expected ')' or ',' in argument list", Severity::ERROR);
                return nullptr;
            }
            this->nextToken();
        }
    }

    this->nextToken();

    return new CallExpressionAST(name, args);
}

ExpressionAST* Parser::parseExpression() {
    ExpressionAST* LHS = this->parsePrimary();
    if (!LHS) return nullptr;
    return this->parseBinOpRHS(0, LHS);
}

ExpressionAST* Parser::parseBinOpRHS(int exprPrec, ExpressionAST* LHS) {
    while (true) {
        std::string binop;
        binop.push_back((char) this->token);
        char c = this->lexer->peek(1);
        if (c == '&' || c == '|' || c == '<' || c == '>' || c == '=') {
            binop.push_back(c);
            this->nextToken();
        }

        int tokPrec = getBinOpPrec(binop);

        if (tokPrec < exprPrec) return LHS;

        this->nextToken();
        auto RHS = this->parsePrimary();
        if (!RHS) return nullptr;

        std::string binop2;
        binop2.push_back((char) this->token);
        c = this->lexer->peek(1);
        if (c == '&' || c == '|' || c == '<' || c == '>' || c == '=') {
            binop2.push_back(c);
        }

        int nextPrec = getBinOpPrec(binop2);
        if (tokPrec < nextPrec) {
            RHS = parseBinOpRHS(tokPrec + 1, RHS);
            if (!RHS) return nullptr;
        }   

        LHS = new BinaryExprAST(getBinOpId(binop), LHS, RHS);
    }
}

ExpressionAST* Parser::parsePrimary() {
    switch (this->token) {
        default:
            log("unknown token when expecting an expression", Severity::ERROR);
            return nullptr;
        case TKN_IDENTIFIER:
            return this->parseIdentifierExpression();
        case TKN_FLOAT_LITERAL:
        case TKN_INT_LITERAL:
            return this->parseNumberExpression();
        case TKN_STRING_LITERAL:
            return this->parseStringExpression();
        case '(':
            return this->parseParenthesisExpression();
    }
}

PrototypeAST* Parser::parsePrototype() {

    int type;

    if (this->token == TKN_LONG) {
        this->nextToken();
        if (this->token == TKN_INT) type = TYPE_LONG;
        else if (this->token == TKN_FLOAT) type = TYPE_DOUBLE;
        else type = TYPE_LONG;
    }
    else {
        if (this->token == TKN_CHAR) type = TYPE_CHAR;
        else if (this->token == TKN_INT) type = TYPE_INT;
        else if (this->token == TKN_FLOAT) type = TYPE_FLOAT;
        else {
            log("Invalid type for function!", Severity::ERROR);
            return nullptr;
        }
    }

    this->nextToken();    

    if (this->token != TKN_IDENTIFIER) {
        log("Expected function name in prototype", Severity::ERROR);
        return nullptr;
    }

    std::string name = this->lexer->getStr();
    this->nextToken();

    if (this->token != '(') {
        log("Expected '(' in prototype", Severity::ERROR);
        return nullptr;
    }

    std::vector<VariableDeclarationAST*> args;
    while (true) {
        this->nextToken();
        if (this->token == TKN_FLOAT || this->token == TKN_INT || this->token == TKN_CHAR || this->token == TKN_LONG || this->token == TKN_CONST) {
            args.push_back(this->parseVariableDeclaration());
        }
        else break;
    }

    if (this->token != ')') {
        log("Expected ')' in prototype", Severity::ERROR);
        return nullptr;
    }

    this->nextToken();

    return new PrototypeAST(name, type, args);
}

VariableDeclarationAST* Parser::parseVariableDeclaration() {
 
    int type;
    bool cnst = false;

    if (this->token == TKN_CONST) {
        cnst = true;
        this->nextToken();
    }

    if (this->token == TKN_LONG) {
        this->nextToken();
        if (this->token == TKN_INT) type = TYPE_LONG;
        else if (this->token == TKN_FLOAT) type = TYPE_DOUBLE;
        else type = TYPE_LONG;
    }
    else {
        if (this->token == TKN_CHAR) type = TYPE_CHAR;
        else if (this->token == TKN_INT) type = TYPE_INT;
        else if (this->token == TKN_FLOAT) type = TYPE_FLOAT;
        else {
            log("Invalid type for function!", Severity::ERROR);
            return nullptr;
        }
    }   

    this->nextToken();

    if (this->token != TKN_IDENTIFIER) {
        log("Expected identifier for variable.", Severity::ERROR);
        return nullptr;
    }

    std::string name = this->lexer->getStr();

    this->nextToken();

    if (this->token == ';' || this->token == ')') {
        return new VariableDeclarationAST(name, type, cnst);
    }

    if (this->token == ',') {
        //for now this is only possible in function declarations, so we return
        return new VariableDeclarationAST(name, type, cnst);
    }

    if (this->token != '=') {
        log("Expected '=' after variable declaration.", Severity::ERROR);
        return nullptr;
    }

    this->nextToken();

    std::string value;

    switch (type) {
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            if (!(this->token == TKN_FLOAT_LITERAL || this->token == TKN_INT_LITERAL)) {
                log("Expected number assignment to floating point value", Severity::ERROR);
                return nullptr;
                break;
            }

            if (this->token == TKN_FLOAT_LITERAL) {
                value = std::to_string(this->lexer->getDVal());
            }
            else {
                value = std::to_string(this->lexer->getLVal());
            }
            break;
        case TYPE_INT:
        case TYPE_LONG:
            if (!(this->token == TKN_FLOAT_LITERAL || this->token == TKN_INT_LITERAL)) {
                log("Expected number assignment to integer point value", Severity::ERROR);
                return nullptr;
                break;
            }

            if (this->token == TKN_FLOAT_LITERAL) {
                value = std::to_string((long long) std::floor(this->lexer->getDVal()));
            }
            else {
                value = std::to_string(this->lexer->getLVal());
            }
            break;
        case TYPE_CHAR:
            if (this->token != TKN_CHAR_LITERAL) {
                log("Expected char value.", Severity::ERROR);
                return nullptr;           
            }
            value.clear();
            value.push_back(this->lexer->getCVal());
    }
    this->nextToken();

    if (this->token != ';') {
        std::string error = "Expected ';' at \"";
        error.append(typeStr(type));
        error.append(" ");
        error.append(name);
        error.append(" = ");
        if (type == TYPE_CHAR) error.append("'");
        error.append(value);
        if (type == TYPE_CHAR) error.append("'");
        error.append("\" <-- here.");
        log(error, Severity::ERROR);
        return nullptr;
    }

    return new VariableDeclarationAST(name, type, cnst, value);
}

void Parser::loop() {
    parseVariableDeclaration();
}