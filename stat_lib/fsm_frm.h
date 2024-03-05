#ifndef FSM_H_INCLUDED
#define FSM_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

using Lexem = std::pair<std::string, std::string>;

void read(char& cache, std::istream& stream);
std::pair<int, Lexem> tick(int state, std::istream& stream, char& cache, std::string& memory);

#endif // FSM_H_INCLUDED