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
	ofstream fout("error.txt");
	cout.rdbuf(fout.rdbuf());

	auto output_setting = "e";
	// auto output_setting = "ve";
	
	lex::LexParser lex_parser(source, output_setting);
	syntax::SyntaxParser syntax_parser(lex_parser, output_setting);
	
	syntax_parser.start();

    return 0;
}
