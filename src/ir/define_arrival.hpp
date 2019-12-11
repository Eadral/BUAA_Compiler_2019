#pragma once

#include "../meow/core.hpp"
#include "flowgraph.hpp"


namespace buaac {

	class DefineArrival {

		map<string, set<string>> in, out, gen, kill;
		set<string> block_names;

	public:
		DefineArrival() {
			
		}

		set<string>& getIn(string block_name) {
			return in[block_name];
		}

		set<string>& getOut(string block_name) {
			return out[block_name];
		}

		set<string>& getGen(string block_name) {
			return gen[block_name];
		}

		set<string>& getKill(string block_name) {
			return kill[block_name];
		}

		void addName(string block_name) {
			block_names.insert(block_name);
		}
		
		void addGen(string block_name, string define_name) {
			block_names.insert(block_name);
			gen[block_name].insert(define_name);
		}

		void addGen(string block_name, vector<string> define_names) {
			for (int i = 0; i < define_names.size(); i++) {
				addGen(block_name, define_names[i]);
			}
		}

		void addKill(string block_name, string define_name) {
			block_names.insert(block_name);
			kill[block_name].insert(define_name);
		}

		void addKill(string block_name, vector<string> define_names) {
			for (int i = 0; i < define_names.size(); i++) {
				addKill(block_name, define_names[i]);
			}
		}

		void generate(FlowGraph& flow_graph) {
			

			while (true) {
				auto in_old = in;
				auto out_old = out;
				auto gen_old = gen;
				auto kill_old = kill;
				
				iterate(flow_graph);

				if (in_old == in
					&& out_old == out
					&& gen_old == gen
					&& kill_old == kill) {
					break;
				}
			}  
			
		}

		void iterate(FlowGraph& flow_graph) {
			for (const auto &block_name: block_names) {
				// const string& block_name = *i;

				// in
				const auto &preds = flow_graph.getPreds(block_name);
				set<string> inMerge;
				for (int j = 0; j < preds.size(); j++) {
					block_names.insert(preds[j]);
					inMerge = setOr(inMerge, out[preds[j]]);
				}
				in[block_name] = inMerge;

				// out
				out[block_name] = setOr(gen[block_name], setSub(in[block_name], kill[block_name]));
				
			}

			// printInOut();
		}

		void printInOut() {
			for (auto block_name : block_names) {
				// const string& block_name = *i;

				print("Gen[{}]: ", block_name);
				for (auto block_name : gen[block_name]) {
					print("{}, ", block_name);
				}
				println("");

				print("Kill[{}]: ", block_name);
				for (auto block_name : kill[block_name]) {
					print("{}, ", block_name);
				}
				println("");
				
				print("In[{}]: ", block_name);
				for (auto block_name : in[block_name]) {
					print("{}, ", block_name);
				}
				println("");

				print("Out[{}]: ", block_name);
				for (auto block_name : out[block_name]) {
					print("{}, ", block_name);
				}
				println("");
				println("");
			}
		}
		
	private:
		
	};
	
}
