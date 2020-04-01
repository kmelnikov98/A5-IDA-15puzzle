#include"PatternDatabase.h"

void PatternDatabase::BuildPatterns(uint64_t& nodesExpanded)
{
	Position initial_pos; //initial position, it is set to the goal state, initially
	int pieceValue = 1;
	initial_pos.cost = 0;
	
	while (pieceValue < BOARD_SIZE) //once you reach pieceValue = 15
	{
		if (pieceValue % 3 == 0) //b/c database is set on 3 pieces, we want to initialize pieceValues based on this.
		{
			position = initial_pos; //reset state
			position.pieceValue[0] = pieceValue - 2; //for ex, pieceValue = 1
			position.pieceValue[1] = pieceValue - 1; //pieceValue = 2
			position.pieceValue[2] = pieceValue; //pieceValue = 3
			SetPosition();
			bfs(nodesExpanded, position);
		}

		pieceValue++;
	}
}

void PatternDatabase::bfs(uint64_t& nodesExpanded, PatternDatabase::Position& initialPos)
{
	std::queue<PatternDatabase::Position> frontier; //need board in order to keep track of position/dup states
	std::set<PatternDatabase::Position> explored;
	PatternDatabase::Position front_pos; //first node; position will be the node initialized 
	PatternDatabase::Position succ_pos;

	frontier.push(initialPos);
	explored.insert(initialPos);

	while (!frontier.empty())
	{
		nodesExpanded++;
		front_pos = frontier.front();
		position = front_pos;

		for(int tile = 0; tile < NUM_PIECES; tile++) // 0 is piece 1, 1 is piece 2. Can extend to more than 2 pieces; all depends on init function.
		{
			for (int move = 0; move < NUM_MOVES; move++)
			{
				if (!IsValidMove(move, tile)) //confirm it is a valid move
				{
					continue;
				}

				MakeMove(move, tile);
				position.cost += 1;
				succ_pos = position; //copy successor state

				if (!explored.count(succ_pos)) //as in, its not found
				{
					explored.insert(succ_pos); //create copy, add back to explored list.
					frontier.push(succ_pos);
				}

				position = front_pos; //go back to original state
			}
		}

		frontier.pop(); //pop front node, which you were working with
	}

	patternDB.insert(explored.begin(), explored.end()); //insert the explored list into the patternDB.
}

void PatternDatabase::SetPosition()
{
	for (int row = 0; row < 4; ++row) //row, so essentially acts as y-coordinate
	{
		for (int col = 0; col < 4; ++col) //column so acts as x-coordinate
		{
			if (position.state[row][col] != position.pieceValue[0] && 
				position.state[row][col] != position.pieceValue[1] &&
				position.state[row][col] != position.pieceValue[2])
			{
				position.state[row][col] = 0;
			}
		}
	}
}

void PatternDatabase::SetPositionFromNode(Node& node)
{
	for (int row = 0; row < 4; ++row) //row, so essentially acts as y-coordinate
	{
		for (int col = 0; col < 4; ++col) //column so acts as x-coordinate
		{
			position.state[row][col] = node.nodeState.state[row][col]; //first, copy the state; O(1) operation

			if (position.state[row][col] != position.pieceValue[0] &&
				position.state[row][col] != position.pieceValue[1] &&
				position.state[row][col] != position.pieceValue[2]) //if position is not one of the piece values, delete
			{
				position.state[row][col] = 0;
			}
		}
	}
}

void PatternDatabase::GetIndexOfTile(int& pieceIndex)
{
	for (int row = 0; row < 4; ++row) //row, so essentially acts as y-coordinate
	{
		for (int col = 0; col < 4; ++col) //column so acts as x-coordinate
		{
			if (position.state[row][col] == position.pieceValue[pieceIndex])
			{
				//flipping these around is wrong; must look into this.
				position.pieceCoordinate[pieceIndex].index[0] = row; //x-coordinate
				position.pieceCoordinate[pieceIndex].index[1] = col; //y-coordinate
				return;
			}
		}
	}
}

//The same functions as given in "Node" do not apply anymore now
bool PatternDatabase::IsValidMove(int& move, int& pieceIndex) //The functions/calculations are completely different now, which is why inheritance is not applied.
{
	GetIndexOfTile(pieceIndex);
	int row_index = position.pieceCoordinate[pieceIndex].index[0];
	int col_index = position.pieceCoordinate[pieceIndex].index[1];
	//Just check if the move left, right, up, or down is == 0. IF it is NOT equal to 0, then it is not a valid move - b/c you are swapping an actual piece.

	switch (move)
	{
		case Node::Move::UP:
			row_index = row_index - 1;
			break;

		case Node::Move::RIGHT:
			col_index = col_index + 1;
			break;

		case Node::Move::DOWN:
			row_index = row_index + 1;
			break;

		case Node::Move::LEFT:
			col_index = col_index - 1;
			break;
	}

	if (row_index > 3 || row_index < 0 || col_index > 3 || col_index < 0)
	{
		return false;
	}

	if (position.state[row_index][col_index] != 0) //Becuase you would be swapping with a piece then.
	{
		return false;
	}

	return true;

}

void PatternDatabase::MakeMove(int& move, int& pieceIndex) //returns index of new zero pos.
{
	//GetIndexOfTile(int& pieceIndex,  //calculates zero position for this node
	int row_index = position.pieceCoordinate[pieceIndex].index[0];
	int col_index = position.pieceCoordinate[pieceIndex].index[1];

	if (move == Node::Move::UP)
	{
		std::swap(position.state[row_index][col_index],
			position.state[row_index - 1][col_index]);
	}
	else if (move == Node::Move::RIGHT)
	{
		std::swap(position.state[row_index][col_index],
			position.state[row_index][col_index + 1]);
	}
	else if (move == Node::Move::DOWN)
	{
		std::swap(position.state[row_index][col_index],
			position.state[row_index + 1][col_index]);
	}
	else if (move == Node::Move::LEFT)
	{
		std::swap(position.state[row_index][col_index],
			position.state[row_index][col_index - 1]);
	}
}
