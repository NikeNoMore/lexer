#include "pch.h"
#include "../stat_lib/lexer_frm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

TEST(TestCaseName, TestName) {
	filebuf fb;
	if (fb.open("test1.txt", ios::in)) {
		istream is(&fb);
		Lexer lex(is);
		vector<Lexem> res;
		while (is) {
			Lexem l = lex.getNextLexem();
			res.push_back(l);
		}
		EXPECT_EQ(res, vector<Lexem> { {"kw", "int"}, { "id", "k" }, { "opeq", "" }, { "num", "3" }, { "semicolon", "" }});
		fb.close();
	}
}
