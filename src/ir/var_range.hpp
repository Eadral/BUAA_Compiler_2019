#pragma once

#include "../meow/core.hpp"

class DefineUseNode : implement fmt::Display {
public:
	int block_index, line_number;
	string block_name;

	DefineUseNode() {}
	
	DefineUseNode(int index, string block, int number) : block_index(index), block_name(block), line_number(number) {}

	// DefineUseNode(string block, int number): block_index(0), block_name(block), line_number(i2a(number)) {}

	std::string toString() {
		return FORMAT("<{}_{}, {}>", block_index, block_name, line_number);
	}


	friend bool operator==(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return std::tie(lhs.block_index, lhs.block_name, lhs.line_number) == std::tie(rhs.block_index, rhs.block_name, rhs.line_number);
	}

	friend bool operator!=(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return !(lhs == rhs);
	}

	friend bool operator<(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return std::tie(lhs.block_index, lhs.block_name, lhs.line_number) < std::tie(rhs.block_index, rhs.block_name, rhs.line_number);
	}

	friend bool operator<=(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return !(rhs < lhs);
	}

	friend bool operator>(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return rhs < lhs;
	}

	friend bool operator>=(const DefineUseNode& lhs, const DefineUseNode& rhs) {
		return !(lhs < rhs);
	}
};

class VarRange {
public:

	// int block_index, block_line_number;
	DefineUseNode first, last;
	VarRange() {}
	VarRange(DefineUseNode node) : first(node), last(node) {}
	VarRange(DefineUseNode first, DefineUseNode last) : first(std::min(first, last)), last(std::max(first, last)) {}

	static VarRange merge(VarRange lhs, VarRange rhs) {
		return VarRange(std::min(lhs.first, rhs.first), std::max(lhs.last, rhs.last));
	}

	bool in(int block_index, int block_line_number) {
		return tie(first.block_index, first.line_number) <= tie(block_index, block_line_number)
			&& tie(block_index, block_line_number) <= tie(last.block_index, last.line_number);
	}

	bool out(int block_index, int block_line_number) {
		return !in(block_index, block_line_number);
	}

private:
	// bool inBlock(int block_index) {
	// 	return first.block_index <= block_index && block_index <= last.block_index;
	// }
	//
	// bool inLine(int line_number) {
	// 	return first.line_number <= line_number && line_number <= last.line_number;
	// }
};
