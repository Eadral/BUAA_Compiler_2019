//
// Created by rudyc on 2019/9/5.
//

#include <iostream>

#include "src/meow/core.hpp"
#include "src/lex/lexparse.hpp"
#include "src/syntax/syntaxparse.hpp"

using namespace std;
using namespace buaac;


int main() {

	
	string source = readFileToString("testfile.txt");
	ofstream fout("output.txt");
	cout.rdbuf(fout.rdbuf());

	lex::LexParser lex_parser(source);
	syntax::SyntaxParser syntax_parser(lex_parser, true);
	
	syntax_parser.start();

    return 0;
}
