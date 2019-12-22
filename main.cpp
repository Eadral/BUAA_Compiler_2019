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

#ifdef WIN32
#define IR_BEFORE "16191051_zhuyucong_ir_before_optimization.txt"
#define IR_AFTER "16191051_zhuyucong_ir_after_optimization.txt"
#else
#define IR_BEFORE "16191051_朱雨聪_优化前中间代码.txt"
#define IR_AFTER "16191051_朱雨聪_优化后中间代码.txt"
#endif

int main(int argc, char **argv) {
	// Output::getInstance()->init();
	// std::locale::global(std::locale(""));
	
	string source = readFileToString("testfile.txt");
	ofstream fout("error.txt");
	cout.rdbuf(fout.rdbuf());

	auto output_setting = "e";
	// auto output_setting = "ve";
	
	lex::LexParser lex_parser(source, output_setting);
	syntax::SyntaxParser syntax_parser(lex_parser, output_setting);
	
	IR ir = syntax_parser.start();

	if (syntax_parser.hasError() || lex_parser.hasError()) {
		return 0;
	}
	
	// ir.output("ir.txt");
	
	Optimizer optimizer(ir);

	IrGen irgen_before(ir);
	irgen_before.output(IR_BEFORE);
	
	optimizer.optimize();
	IR optimized_ir = optimizer.getIR();

	IrGen irgen(optimized_ir);
	irgen.output(IR_AFTER);
	
	MIPS mips(optimized_ir);

	mips.gen();
	
    return 0;
}
