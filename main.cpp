//
// Created by rudyc on 2019/9/5.
//

#include <iostream>

#include "src/meow/core.hpp"

#include <regex>

#include "src/lex/token.hpp"
#include "src/lex/lexparse.hpp"

using namespace std;
using namespace buaac;


int main() {

	string source = readFileToString("testfile.txt");
	ofstream fout("output.txt");


	lex::LexParser lexParser(source);
	
	lex::LexResult lexResult;
	while ((lexResult = lexParser.lookToken()).isOk()) {
		lexParser.eatToken();
		auto token = lexResult.unwrap();
		fout << token << endl;
		
	}

    return 0;
}
