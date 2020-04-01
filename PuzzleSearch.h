#pragma once
#include"Node.h"
#include"PatternDatabase.h"
#include<vector>
#include <list>
#include<queue>
#include<iostream>
#include<string>
#include<set>

struct Cost
{
	int g;
	int h;
};

class FifteenPuzzleSearch
{
	private:
		uint8_t goalState[4][4] = {{1, 2, 3, 4},
								   {5, 6, 7, 8},
								   {9, 10, 11, 12},
								   {13, 14, 15, 0}};

	public:

		std::vector<std::string> IDA_root(Node& initialNode, uint64_t& nodesExpanded, PatternDatabase& DB); //nodes Expanded passed in here

		int IDA(Node& node, int& threshold, uint64_t& nodesExpanded,
				std::set<Node::State>& explored, std::vector<std::string>& path, Cost cost, PatternDatabase& DB);
		//pass in reference of exploredList, and nodesExpanded; global variables

		int PatternHeuristic(Node& node, PatternDatabase& DB);

		void RandomizeInitialState(Node& initialNode, int num_rand_moves = 50);

		uint8_t Manhattan(Node& node);

		std::string GetMoveAsString(int& move);
};
