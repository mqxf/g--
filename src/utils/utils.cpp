#include <string>
#include <iostream>
#include <memory>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "utils.h"
#include "../main.h"
#include "../lexer/lexer.h"
#include "../parser/AST.h"

using namespace llvm;

char strToEsc(std::string str) {
    if (str.length() <= 1) return 0;
    if (str[0] != '\\') return 0;
    char c = str[1];

    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return c;
        default: return c;
    }

    return c;
}

std::string strFormat(std::string str) {
    std::string out;
    size_t len = str.length();

    size_t i = 0;
    while (str[i] != '\0' && i < len) {
        if (str[i] == '\\') {
            char escape = strToEsc(str.substr(i, 2));
            out.push_back(escape);
            i += 2;
        }
        else {
            out.push_back(str[i]);
            i += 1;
        }
    }
    return out;
}

void log(std::string message, Severity severity) {
    if (severity == Severity::ERROR) {
        std::cerr << message << std::endl;
    }
    else if (severity == Severity::WARNING && !args->silent) {
        std::cout << message << std::endl;
    }
    else if (severity == Severity::LOG && args->verbose) {
        std::cout << message << std::endl;
    }
}

int getBinOpPrec(std::string str) { 
    if(str == "==") return 20;
    if(str == "=>") return 20;
    if(str == "=<") return 20;
    if(str == "<") return 20;
    if(str == ">") return 20;
    if(str == "%") return 40;
    if(str == "&&") return 20;
    if(str == "||") return 20;
    if(str == ">>") return 30;
    if(str == "<<") return 30;
    if(str == "&") return 20;
    if(str == "|") return 20;
    if(str == "^") return 20;
    if(str == "+") return 20;
    if(str == "-") return 20;
    if(str == "*") return 40;
    if(str == "/") return 40;
    if(str == "=") return 10;
    return -1;
}

int getBinOpId(std::string str) {
    if(str == "==") return OP_EEQ;
    if(str == "<=") return OP_LTE;
    if(str == ">=") return OP_GTE;
    if(str == "<") return OP_LT;
    if(str == ">") return OP_GT;
    if(str == "%") return OP_MOD;
    if(str == "&&") return OP_AND;
    if(str == "||") return OP_OR;
    if(str == ">>") return OP_RSHIFT;
    if(str == "<<") return OP_LSHIFT;
    if(str == "&") return OP_BAND;
    if(str == "|") return OP_BOR;
    if(str == "^") return OP_XOR;
    if(str == "+") return OP_ADD;
    if(str == "-") return OP_SUB;
    if(str == "*") return OP_MTP;
    if(str == "/") return OP_DIV;
    if(str == "=") return OP_EQ;
    return -1;
}

std::string typeStr(int type) {
    switch(type) {
        case TYPE_LONG: return "long";
        case TYPE_DOUBLE: return "long float";
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_CHAR: return "char";
    }
    return "";
}

void initModule() {
  context = std::make_unique<LLVMContext>();
  mod = std::make_unique<Module>("JIT", *context);
  builder = std::make_unique<IRBuilder<>>(*context);
}