#include "Lexer.h"
#include "fsm.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


Lexer::Lexer(istream& stream) : state{ 0 }, stream{ stream }
{
    read(cache, stream);
}

Lexer::~Lexer()
{
    //dtor
}

Lexem Lexer::getNextLexem() {
    Lexem lex = LEX_EMPTY;
    string memory;
    while (lex.first == LEX_EMPTY.first and state >= 0) {
        auto res = tick(state, stream, cache, memory);
        state = res.first;
        lex = res.second;
    }
    return lex;
}