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

class NodeRange {
public:

	DefineUseNode first, last;
	NodeRange() {}
	NodeRange(DefineUseNode node) : first(node), last(node) {}
	NodeRange(DefineUseNode first, DefineUseNode last) : first(std::min(first, last)), last(std::max(first, last)) {}

	static NodeRange merge(NodeRange lhs, NodeRange rhs) {
		return NodeRange(std::min(lhs.first, rhs.first), std::max(lhs.last, rhs.last));
	}

	bool in(int block_index, int block_line_number) {
		return tie(first.block_index, first.line_number) <= tie(block_index, block_line_number)
			&& tie(block_index, block_line_number) <= tie(last.block_index, last.line_number);
	}

	bool out(int block_index, int block_line_number) {
		return !in(block_index, block_line_number);
	}
	
};

class VarRange {
public:

	// int block_index, block_line_number;
	vector<NodeRange> ranges;
	VarRange() {}
	VarRange(NodeRange range) {
		ranges.push_back(range);
	}
	// VarRange(DefineUseNode first, DefineUseNode last) {
	// 	ranges.push_back(NodeRange(first, last));
	// }
	
	void addRange(NodeRange range) {
		ranges.push_back(range);
	}

	bool in(int block_index, int block_line_number) {
		for (auto &range: ranges) {
			if (range.in(block_index, block_line_number))
				return true;
		}
		return false;
	}

	bool out(int block_index, int block_line_number) {
		return !in(block_index, block_line_number);
	}
};
