#ifndef _AST_H
#define _AST_H

#include <string>
#include <vector>
#include <llvm/IR/Value.h>
#include "../utils/utils.h"

enum BinOp {
    OP_ADD = 1, 
    OP_SUB = 2,
    OP_MTP = 3,
    OP_DIV = 4,
    OP_EQ = 5,
    OP_EEQ = 6,
    OP_GT = 7,
    OP_LT = 8,
    OP_GTE = 9,
    OP_LTE = 10,
    OP_LSHIFT = 11,
    OP_RSHIFT = 12,
    OP_BAND = 13,
    OP_BOR = 14,
    OP_XOR = 15,
    OP_AND = 16,
    OP_OR = 17,
    OP_MOD = 18,
};

enum DataType {
    TYPE_CHAR = 1,
    TYPE_INT = 2,
    TYPE_LONG = 3,
    TYPE_FLOAT = 4,
    TYPE_DOUBLE = 5,
    TYPE_VOID = 6,
};

class ExpressionAST {
    private:

    public:
        virtual ~ExpressionAST() {}
        virtual Value* codegen() = 0;
};

class NumberExpressionAST : public ExpressionAST {
    private:
        bool fl;
        double fVal;
        long long lVal;

    public:
        NumberExpressionAST(double fVal);
        NumberExpressionAST(long long lVal);
        Value* codegen() override;
};

class StringExpressionAST : public ExpressionAST {
    private:
        std::string value;

    public:
        StringExpressionAST(std::string value);
        Value* codegen() override;
};

class VariableExpressionAST : public ExpressionAST {
    private:
        std::string name;
        
    public:
        VariableExpressionAST(const std::string name);
        Value* codegen() override;
};

class VariableDeclarationAST : public ExpressionAST {
    private:
        std::string name;
        int type;
        bool declared;
        bool cnst;
        std::string value;
    public:
        VariableDeclarationAST(std::string name, int type, bool cnst);
        VariableDeclarationAST(std::string name, int type, bool cnst, std::string value);
        Value* codegen() override;
};

class BinaryExprAST : public ExpressionAST {
    private:
        int op;
        ExpressionAST* LHS;
        ExpressionAST* RHS;

    public:
        BinaryExprAST(int op, ExpressionAST* LHS, ExpressionAST* RHS);
        Value* codegen() override;
};

class CallExpressionAST : public ExpressionAST {
    private:
        std::string callee;
        std::vector<ExpressionAST*> args;
    public:
        CallExpressionAST(std::string callee, std::vector<ExpressionAST*> args);
        Value* codegen() override;
};

//Function AST

class PrototypeAST {
    private:
        std::string name;
        int returnType;
        std::vector<VariableDeclarationAST*> args;

    public:
        PrototypeAST(std::string name, int returnType, std::vector<VariableDeclarationAST*> args);
};

class FunctionAST {
    private:
        PrototypeAST* prototype;
        ExpressionAST* body;

    public:
        FunctionAST(PrototypeAST* prototype, ExpressionAST* body);
};

#endif