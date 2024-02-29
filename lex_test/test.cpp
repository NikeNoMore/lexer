#include "pch.h"
#include "../stat_lib/lexer_frm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

TEST(TestCaseName, TestName1) {
	filebuf fb;
	if (fb.open("test1.txt", ios::in)) {
		istream is(&fb);
		Lexer lex(is);
		vector<Lexem> res;
		while (is) {
			Lexem l = lex.getNextLexem();
			res.push_back(l);
		}
		vector<Lexem> check{ {"kw", "int"}, { "id", "k" }, { "opeq", "" }, { "num", "3" }, { "semicolon", "" } };
		ASSERT_EQ(res.size(), check.size());
		for (int i = 0; i < res.size(); i++) {
			EXPECT_EQ(res[i], check[i]);
		}
		fb.close();
	}
}

TEST(TestCaseName, TestName2) {
	filebuf fb;
	if (fb.open("test2.txt", ios::in)) {
		istream is(&fb);
		Lexer lex(is);
		vector<Lexem> res;
		while (is) {
			Lexem l = lex.getNextLexem();
			res.push_back(l);
		}
		vector<Lexem> check{ {"kw", "int"}, { "id", "a" }, { "opeq", "" }, { "num", "1" }, { "semicolon", "" },
		{"kw", "int"}, { "id", "b" }, { "opeq", "" }, { "num", "2" }, { "semicolon", "" },
		{"kw", "int"}, { "id", "c" }, { "opeq", "" }, { "id", "a" }, {"opplus", ""}, {"id", "b"}, {"semicolon", ""}};
		ASSERT_EQ(res.size(), check.size());
		for (int i = 0; i < res.size(); i++) {
			EXPECT_EQ(res[i], check[i]);
		}
		fb.close();
	}
}
