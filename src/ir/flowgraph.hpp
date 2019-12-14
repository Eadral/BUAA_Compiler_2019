#pragma once

#include "../meow/core.hpp"
#include <map>
#include <unordered_map>


namespace buaac {
	class FlowGraph {

		std::unordered_map<string, vector<string>> nextNodes;
		std::unordered_map<string, vector<string>> predNodes;
		std::unordered_map<string, int> label_to_id;

		// vector<string> entrys;
		
	public:
		// const string entry = "entry";
		// const string exit = "exit";

		std::unordered_map<string, vector<string>> getNextNodes() {
			return nextNodes;
		}

		void addId(string label, int id) {
			label_to_id[label] = id;
		}

		int getId(string label) {
			return label_to_id[label];
		}
		
		void addEdge(string prev, string next) {
			// if (nextNodes.find(prev) == nextNodes.end()) {
			// 	nextNodes[prev] = vector<string>();
			// }
			nextNodes[prev].push_back(next);
			
			// if (predNodes.find(next) == predNodes.end()) {
			// 	predNodes[next] = vector<string>();
			// }
			predNodes[next].push_back(prev);
		}

		// void addEntry(string entry) {
		// 	entrys.push_back(entry);
		// }
		//
		// vector<string> getEntryNames() {
		// 	return entrys;
		// }
		
		string getExitName(string entry) {
			return FORMAT("return_{}", entry);
		}

		vector<string> getNexts(string prev) {
			return nextNodes[prev];
		}

		vector<string> getPreds(string next) {
			return predNodes[next];
		}

		void printGraph() {
			for (auto i = nextNodes.begin(); i != nextNodes.end(); i++) {
				auto x = i->first;
				auto y = i->second;
				print("{} -> ", x);
				for (int j = 0; j < y.size();j++) {
					if (j != y.size()-1) {
						print("{}, ", y[j]);
					} else {
						println("{}", y[j]);
					}
				}
			}
		}
		
	};
}
