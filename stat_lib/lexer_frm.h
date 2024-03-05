#include "pch.h"
#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>

using Lexem = std::pair<std::string, std::string>;
const Lexem LEX_EMPTY = { "", "" };
const Lexem LEX_ERROR = { "error", "" };
const Lexem LEX_EOF = { "end", "" };

class Lexer
{
public:
    Lexer(std::istream&);
    virtual ~Lexer();
    std::pair<std::string, std::string> getNextLexem();

protected:

private:
    char cache;
    int state;
    std::istream& stream;
};
#endif // LEXER_H
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
