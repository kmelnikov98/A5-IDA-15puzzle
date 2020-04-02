#include"PuzzleSearch.h"
#include"PatternDatabase.h"
#include"Node.h"
#include<iostream>
#include<random>
#include<chrono>
#include<algorithm>

void PrintMove(std::string& move);

int main(void)
{
	Node initialNode;
	FifteenPuzzleSearch fifteenPuzzle;
	uint64_t nodesExpanded = 0;
	uint64_t nodesExpandedPatternDB = 0;
	int numScrambleMoves;
	std::vector<std::string> sol;
	int move = 4; //some random initial value, which is not a move
	PatternDatabase patternDB;

	std::cout << "Number of moves to make when scrambling the board: ";
	std::cin >> numScrambleMoves;
	std::cout << std::endl;

	patternDB.BuildPatterns(nodesExpandedPatternDB);
	std::cout << "Size of DB: " << nodesExpandedPatternDB << std::endl;

	initialNode.PrintState();
	initialNode.FindZeroPos();
	std::cout << std::endl << "Scrambled Board - Initial State:" << std::endl;
 	fifteenPuzzle.RandomizeInitialState(initialNode, numScrambleMoves);
	Node::State init_state = initialNode.nodeState;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	sol = fifteenPuzzle.IDA_root(initialNode, nodesExpanded, patternDB);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << std::endl << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
	std::cout << "Nodes Expanded: " << nodesExpanded << std::endl;

	std::cout << std::endl << std::endl << "Solution: ";
	std::for_each(sol.rbegin(), sol.rend(), PrintMove);
	std::cout << std::endl;

	std::cout << std::endl;
	initialNode.nodeState = init_state;

	std::cout << "Now, try using the solution to get the goal!" << std::endl;
	std::cout << "Moves: 0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT" << std::endl;
	std::cout << "press -1 to exit program, -2 to reset" << std::endl;

	while (true)
	{
		std::cin >> move;

		if (move == -1)
		{
			std::cout << "Exiting program." << std::endl;
			break;
		}

		if (move == -2)
		{
			initialNode.nodeState = init_state;
			initialNode.PrintState();
			std::cout << "Reset the current board." << std::endl;
			continue;
		}

		if (initialNode.IsValidMove(move))
		{
			initialNode.MakeMove(move);
			initialNode.PrintState();
		}
		else
		{
			std::cout << "Invalid move." << std::endl;
		}

	}
}

void PrintMove(std::string& move)
{
	std::cout << move << " ";
}
