#ifndef FSM_H_INCLUDED
#define FSM_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

using Lexem = std::pair<std::string, std::string>;

const Lexem LEX_EMPTY = { "", "" };
const Lexem LEX_ERROR = { "error", "" };
const Lexem LEX_EOF = { "end", "" };

void read(char& cache, std::istream& stream);
std::pair<int, Lexem> tick(int state, std::istream& stream, char& cache, std::string& memory);

#endif // FSM_H_INCLUDED