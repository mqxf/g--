#include <string>
#include <vector>
#include "AST.h"

NumberExpressionAST::NumberExpressionAST(double fVal) {
    this->fVal = fVal;
}

NumberExpressionAST::NumberExpressionAST(long long lVal) {
    this->lVal = lVal;
}

StringExpressionAST::StringExpressionAST(std::string value) {
    this->value = value;
}

VariableExpressionAST::VariableExpressionAST(std::string name) {
    this->name = name;
}

VariableDeclarationAST::VariableDeclarationAST(std::string name, int type, bool cnst) {
    this->name = name;
    this->type = type;
    this->cnst = cnst;
    this->declared = false;
}

VariableDeclarationAST::VariableDeclarationAST(std::string name, int type, bool cnst, std::string value) {
    this->name = name;
    this->type = type;
    this->value = value;
    this->cnst = cnst;
    this->declared = true;
}

BinaryExprAST::BinaryExprAST(int op, ExpressionAST* LHS, ExpressionAST* RHS) {
    this->op = op;
    this->LHS = LHS;
    this->RHS = RHS;
}

CallExpressionAST::CallExpressionAST(std::string callee, std::vector<ExpressionAST*> args) {
    this->callee = callee;
    this->args = args;
}

PrototypeAST::PrototypeAST(std::string name, int returnType, std::vector<VariableDeclarationAST*> args) {
    this->name = name;
    this->returnType = returnType;
    this->args = args;
}

FunctionAST::FunctionAST(PrototypeAST* prototype, ExpressionAST* body) {
    this->prototype = prototype;
    this->body = body;
}