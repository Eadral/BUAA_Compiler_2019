//
// Created by rudyc on 2019/9/5.
//

#include <iostream>

#include "src/meow/core.hpp"

#include <regex>

#include "src/lex/token.hpp"

using namespace std;
using namespace buaac;


int main() {

	auto source = readFileToString("../../../test.c");

	auto a = lex::eat(source).unwrap();
	cout << a << endl;

	
	
	cout << source << endl;


    return 0;
}
