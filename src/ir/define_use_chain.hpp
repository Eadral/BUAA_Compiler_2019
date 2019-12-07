#pragma once

#include "../meow/core.hpp"
#include "define_arrival.hpp"

namespace buaac {

	
	class DefineUseNode : implement fmt::Display {
	public:
		string block, number;

		DefineUseNode(string block, int number): block(block), number(i2a(number)) {}

		std::string toString() {
			return FORMAT("<{}, {}>", block, number);
		}

		
		friend bool operator==(const DefineUseNode& lhs, const DefineUseNode& rhs) {
			return lhs.block == rhs.block
				&& lhs.number == rhs.number;
		}

		friend bool operator!=(const DefineUseNode& lhs, const DefineUseNode& rhs) {
			return !(lhs == rhs);
		}

		friend bool operator<(const DefineUseNode& lhs, const DefineUseNode& rhs) {
			if (lhs.block < rhs.block)
				return true;
			if (rhs.block < lhs.block)
				return false;
			return lhs.number < rhs.number;
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

	
	class Chain : implement fmt::Display {
		using Node = DefineUseNode;
	public:
		Chain(Node def): def(def) {}
		
		Node def;
		vector<Node> uses;

		std::string toString() {
			std::ostringstream buf;
			buf << "{" << def;
			for (int i = 0; i < uses.size(); i++) {
				buf << ", " << uses[i];
			}
			buf << "}";
			return buf.str();
		}

		bool conflict(Chain chain) {
			auto rhs = getDefAndUsesSet();
			auto lhs = chain.getDefAndUsesSet();
			for (auto i = rhs.begin(); i != rhs.end(); i++) {
				if (found(lhs, *i)) {
					return true;
				}
			}
			return false;
		}

		set<Node> getDefAndUsesSet() {
			set<Node> ans;
			ans.insert(ans.begin(), def);
			for (int i = 0; i < uses.size(); i++) {
				ans.insert(uses[i]);
			}
			return ans;
		}
		
		friend bool operator==(const Chain& lhs, const Chain& rhs) {
			return lhs.def == rhs.def
				&& lhs.uses == rhs.uses;
		}

		friend bool operator!=(const Chain& lhs, const Chain& rhs) {
			return !(lhs == rhs);
		}

		friend bool operator<(const Chain& lhs, const Chain& rhs) {
			if (lhs.def < rhs.def)
				return true;
			if (rhs.def < lhs.def)
				return false;
			return lhs.uses < rhs.uses;
		}

		friend bool operator<=(const Chain& lhs, const Chain& rhs) {
			return !(rhs < lhs);
		}

		friend bool operator>(const Chain& lhs, const Chain& rhs) {
			return rhs < lhs;
		}

		friend bool operator>=(const Chain& lhs, const Chain& rhs) {
			return !(lhs < rhs);
		}
	};

	class Web : implement fmt::Display  {
	public:
		set<Chain> chains;

		std::string toString() {
			std::ostringstream buf;
			buf << "{";
			for (auto i = chains.begin(); i != chains.end(); i++) {
				buf << TOSTR(*i) << ", ";
			}
			buf << "}";
			return buf.str();
		}

		bool canAdd(Chain chain) {
			for (auto i = chains.begin(); i != chains.end(); i++) {
				Chain this_chain = *i;
				if (this_chain.conflict(chain)) {
					return true;
				}
			}
			return false;
		}
		
		friend bool operator==(const Web& lhs, const Web& rhs) {
			return lhs.chains == rhs.chains;
		}

		friend bool operator!=(const Web& lhs, const Web& rhs) {
			return !(lhs == rhs);
		}

		friend bool operator<(const Web& lhs, const Web& rhs) {
			return lhs.chains < rhs.chains;
		}

		friend bool operator<=(const Web& lhs, const Web& rhs) {
			return !(rhs < lhs);
		}

		friend bool operator>(const Web& lhs, const Web& rhs) {
			return rhs < lhs;
		}

		friend bool operator>=(const Web& lhs, const Web& rhs) {
			return !(lhs < rhs);
		}
	};
	
	class DefineUseChain {
		using Node = DefineUseNode;

		// using Node = string;
		
		DefineArrival define_arrival_;

		// map<string, set<Node>> def_nodes, use_nodes;
		
		map<string, set<Node>> ident_to_defs, ident_to_uses;
		set<string> idents;

		map<string, set<Web>> ident_to_webs;

	public:
		DefineUseChain() {
			
		}

		void generate(FlowGraph &flow_graph) {
			define_arrival_.generate(flow_graph);
			genWebs();
			
		}

		void genWebs() {
			for (auto i = idents.begin(); i != idents.end(); i++) {
				genWeb(*i);
			}
		}

		void genWeb(string ident) {
			vector<Chain> chains;
			auto defs = ident_to_defs[ident];
			auto uses = ident_to_uses[ident];
			
			for (auto i = defs.begin(); i != defs.end(); i++) {
				chains.push_back(Chain(*i));
			}

			for (auto use = uses.begin(); use != uses.end(); use++) {
				for (int chain_i = 0; chain_i < chains.size(); chain_i++) {
					if (found(define_arrival_.getIn(use->block), chains[chain_i].def.toString())) {
						chains[chain_i].uses.push_back(*use);
					}
				}
			}

			vector<Web> webs;
			for (int i = 0; i < chains.size(); i++) {
				bool flag = true;
				for (int j = 0; j < webs.size(); j++) {
					if (webs[j].canAdd(chains[i])) {
						webs[j].chains.insert(chains[i]);
						flag = false;
						break;
					}
				}
				if (flag) {
					Web web;
					web.chains.insert(chains[i]);
					webs.push_back(web);
				}
			}

			for (int i = 0; i < webs.size(); i++) {
				ident_to_webs[ident].insert(webs[i]);
			}
			
			
		}

		void printChain() {
			reAddDef();
			define_arrival_.printInOut();

			for (auto i = idents.begin(); i != idents.end(); i++) {
				print("{}: ", *i);
				auto webs = ident_to_webs[*i];
				for (auto j = webs.begin(); j != webs.end(); j++) {
					println("W{}", *j);
				}
			}
		}

		void reAddDef() {
			for (auto i = idents.begin(); i != idents.end(); i++) {
				auto def_nodes = ident_to_defs[*i];
				for (auto j = def_nodes.begin(); j != def_nodes.end(); j++) {
					for (auto k = def_nodes.begin(); k != def_nodes.end(); k++) {
						if (*j != *k) {
							define_arrival_.addKill(j->block, TOSTR(*k));
						}
					}
				}
			}
		}
		
		void addDef(string block_name, int number, string ident) {
			idents.insert(ident);
			Node new_node{ block_name, number };
			
			auto define_blocks = ident_to_defs[ident];
			for (auto i = define_blocks.begin(); i != define_blocks.end(); i++) {
				if (*i != new_node) {
					define_arrival_.addKill(block_name, TOSTR(*i) );
				}
			}
			ident_to_defs[ident].insert(Node(block_name, number));
			define_arrival_.addGen(block_name, new_node.toString());
			
		}

		// string getNodeName(string block, string number) {
		// 	return FORMAT("<{}, {}>", block, number);
		// }

		void addUse(string block_name, int number, string ident) {
			idents.insert(ident);
			Node new_node{ block_name, number };

			ident_to_uses[ident].insert(new_node);
		}

		
	private:
		
	};
}
