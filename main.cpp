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

	auto source = readFileToString("testfile.txt");
	ofstream fout("output.txt");

	
	lex::LexResult lexResult;
	while ((lexResult = lex::eat(source)).isOk()) {
		auto token = lexResult.unwrap();
		fout << token.output() << endl;
	}

	
	fout << source;


    return 0;
}
