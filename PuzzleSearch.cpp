#include"PuzzleSearch.h"
#include"PatternDatabase.h"
#include<random>
#include <algorithm>
#include <list>
#include<queue>
#include<cmath>

std::string FifteenPuzzleSearch::GetMoveAsString(int& move)
{
	std::string str_move;

	switch (move)
	{
	case Node::Move::UP:
		str_move = "UP";
		break;

	case Node::Move::RIGHT:
		str_move = "RIGHT";
		break;

	case Node::Move::DOWN:
		str_move = "DOWN";
		break;

	case Node::Move::LEFT:
		str_move = "LEFT";
		break;
	}

	return str_move;
}

bool MoveIsResetting(int& currMove, int& lastMove) //if the current move is the opposite of last move, then it would undo
{
	if (lastMove == -1)
	{
		return false;
	}

	switch (currMove)
	{
	case Node::Move::UP:
		if (lastMove == Node::Move::DOWN)
			return true;
		break;

	case Node::Move::RIGHT:
		if (lastMove == Node::Move::LEFT)
			return true;
		break;

	case Node::Move::DOWN:
		if (lastMove == Node::Move::UP)
			return true;
		break;

	case Node::Move::LEFT:
		if (lastMove == Node::Move::RIGHT)
			return true;
		break;
	}

	return false;
}

//starting from goal state, randomly shuffle the array
//Doing this, we guarantee that its a solvable state, because we are making valid moves from goal state
void FifteenPuzzleSearch::RandomizeInitialState(Node& initialNode, int num_rand_moves)
{
	//code adapted from: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_int_distribution<> rand_dis(0, 3);
	initialNode.FindZeroPos();
	int currentMove;
	int lastMove = -1;
	std::string path;

	for (int numMoves = 0; numMoves < num_rand_moves; numMoves++) //do 100 random moves
	{
		currentMove = rand_dis(gen); //0 corresponds to up, 1 to right, 2 to down, 3 to left

		while (MoveIsResetting(currentMove, lastMove))
		{
			currentMove = rand_dis(gen); //Regenerate the move, if its repeating
		}

		if (initialNode.IsValidMove(currentMove))
		{
			initialNode.MakeMove(currentMove);
			initialNode.FindZeroPos();
			path = path + " " + GetMoveAsString(currentMove);
			lastMove = currentMove;
		}
		else
		{
			--numMoves; //not a valid move, then dont increment counter
		}
	}

	//initialNode.nodeState.g = 0; //initially 0, because no moves have been made. 
	//initialNode.nodeState.h = Manhattan(initialNode);

	initialNode.PrintState();
	std::cout << std::endl << "Scrambled moves:";
	std::cout << path;
}

int FifteenPuzzleSearch::PatternHeuristic(Node& node, PatternDatabase& DB)
{
	int cost = 0; //return value
	int pieceValue = 1;
	std::set<PatternDatabase::Position>::iterator pos_it;

	while (pieceValue < BOARD_SIZE) //once you reach pieceValue = 15
	{
		if (pieceValue % 3 == 0) //b/c database is set on 3 pieces, we want to initialize pieceValues based on this.
		{
			DB.position.pieceValue[0] = pieceValue - 2; //for ex, pieceValue = 1
			DB.position.pieceValue[1] = pieceValue - 1; //pieceValue = 2
			DB.position.pieceValue[2] = pieceValue; //pieceValue = 3
			DB.SetPositionFromNode(node); //set a new position, derived from the node.
			pos_it = DB.patternDB.find(DB.position);
			cost += pos_it->cost;
		}

		pieceValue++;
	}

	return cost;
}

//adapted from https://stackoverflow.com/questions/12526792/manhattan-distance-in-a
uint8_t FifteenPuzzleSearch::Manhattan(Node& node) //might be good to define outside spectrum of node.
{
	uint8_t mhd = 0;

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			uint8_t value = node.nodeState.state[x][y];

			if (value != 0) //ignore 0.
			{
				int targetX = (value - 1) / 4; // expected x-coordinate (row)
				int targetY = (value - 1) % 4; // expected y-coordinate (col)
				int dx = x - targetX; // x-distance to expected coordinate
				int dy = y - targetY; // y-distance to expected coordinate
				mhd += abs(dx) + abs(dy);
			}
		}
	}
	return mhd;
}

std::vector<std::string> FifteenPuzzleSearch::IDA_root(Node& initialNode, uint64_t& nodesExpanded, PatternDatabase& DB)
{
	std::set <Node::State> explored;
	Node::State start_state = initialNode.nodeState; //keep track of our original state
	Cost initialCost;
	std::vector<std::string> solution;
	int retBound;

	//initialize all values here: cost, threshold, explored list
	initialCost.g = 0;
	initialCost.h = PatternHeuristic(initialNode, DB);
	int threshold = initialCost.g + initialCost.h; //threshold is now f-value of initial node
	explored.insert(start_state); //push initial position; g and h should already be initialized

	while (true)
	{
		retBound = IDA(initialNode, threshold, nodesExpanded, explored, solution, initialCost, DB); //returns a solution

		if (initialNode.IsGoalState(goalState))
		{
			return solution;
		}

		initialNode.nodeState = start_state;
		threshold = retBound; //set new threshold
	}
}

int FifteenPuzzleSearch::IDA(Node& node, int& threshold, uint64_t& nodesExpanded,
		std::set<Node::State>& explored, std::vector<std::string>& path, Cost cost, PatternDatabase& DB)
{
	Node::State start_state = node.nodeState;
	Cost start_cost = cost;
	Node::State succ_state;
	int retBound;
	int min = -1;

	nodesExpanded++;

	if (cost.g + cost.h > threshold) //check if f > threshold; if this is the case, then cut-off the branch
	{
		return (cost.g + cost.h);
	}

	if (node.IsGoalState(goalState))
	{
		return -1; //stop branch, if goal is found
	}

	for (int move = 0; move < NUM_MOVES; move++)
	{
		if (!node.IsValidMove(move)) //confirm it is a valid move
		{
			continue;
		}

		node.MakeMove(move);
		cost.g = cost.g + 1;
		cost.h = FifteenPuzzleSearch::PatternHeuristic(node, DB);

		succ_state = node.nodeState; //copy successor state

		if (!explored.count(succ_state)) //as in. its not found
		{
			explored.insert(succ_state); //add position back to explored list
			retBound = IDA(node, threshold, nodesExpanded, explored, path, cost, DB); //recursively go down a specific depth
			path.push_back(GetMoveAsString(move));

			if (node.IsGoalState(goalState))
			{
				return -1; //returns the current move, even if it is garbage. (for ex, sol could be found, (DOWN DOWN), but, you are at current move LEFT, so it adds DOWN LEFT DOWN also.
			}

			if (min == -1) //first successor
			{
				min = retBound;
			}

			if (min < retBound)
			{
				min = retBound;
			}

			explored.erase(succ_state);
			path.pop_back();
		}

		node.nodeState = start_state; //go back to original state
		cost = start_cost; //reset cost to the start_cost
	}

	return min;
}