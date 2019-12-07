//
// Created by rudyc on 2019/9/5.
//

#include <iostream>

#include "src/meow/core.hpp"
#include "src/lex/lexparse.hpp"
#include "src/syntax/syntaxparse.hpp"
#include "src/output_helper.hpp"
#include "src/ir/ir.hpp"
#include "src/ir/ir_output.hpp"
#include "src/ir/optimizer.hpp"
#include "src/gen/mips.hpp"

using namespace std;
using namespace buaac;


int main() {
	// Output::getInstance()->init();
	
	string source = readFileToString("testfile.txt");
	ofstream fout("output.txt");
	// cout.rdbuf(fout.rdbuf());

	auto output_setting = "";
	// auto output_setting = "ve";
	
	lex::LexParser lex_parser(source, output_setting);
	syntax::SyntaxParser syntax_parser(lex_parser, output_setting);
	
	IR ir = syntax_parser.start();

	// ir.output("ir.txt");
	

	Optimizer optimizer(ir);
	optimizer.optimize();
	IR optimized_ir = optimizer.getIR();

	IrGen irgen(optimized_ir);
	irgen.output("ir.txt");
	
	MIPS mips(optimized_ir);

	mips.gen();
	
    return 0;
}
