#include <vector>
#include <string>
#include "c--.h"
#include "../main.h"
#include "../file/file.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

std::string compile(std::string input) {

    Lexer* lexer = new Lexer(input);
    Parser* parser = new Parser(lexer);
    parser->loop();
    return "";
}

void compileFile() {
    File* input = new File(args->inputFilename);
    File* output = new File(args->outputFilename);

    compile(input->read());
    //output->write(compile(input->read()));
}

