#include"Node.h"
#include<iostream>

void Node::PrintState()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << (unsigned)nodeState.state[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

bool Node::IsValidMove(int& move)
{
	FindZeroPos();
	int y_index = col_zero_pos;
	int x_index = row_zero_pos;
	bool isValid = true;

	switch (move)
	{
		case UP:
			x_index = row_zero_pos - 1;
			break;

		case RIGHT:
			y_index = col_zero_pos + 1;
			break;

		case DOWN:
			x_index = row_zero_pos + 1;
			break;

		case LEFT:
			y_index = col_zero_pos - 1;
			break;
	}

	if (x_index > 3 || x_index < 0 || y_index > 3 || y_index < 0)
	{
		isValid = false;
		return isValid;
	}

	return isValid;
}

void Node::MakeMove(int& move) //returns index of new zero pos.
{
	FindZeroPos(); //calculates zero position for this node

	if (move == UP)
	{
		std::swap(Node::nodeState.state[row_zero_pos][col_zero_pos],
							nodeState.state[row_zero_pos - 1][col_zero_pos]);
	}
	else if (move == RIGHT)
	{
		std::swap(Node::nodeState.state[row_zero_pos][col_zero_pos],
							nodeState.state[row_zero_pos][col_zero_pos + 1]);
	}
	else if (move == DOWN)
	{
		std::swap(Node::nodeState.state[row_zero_pos][col_zero_pos],
							nodeState.state[row_zero_pos + 1][col_zero_pos]);
	}
	else if (move == LEFT)
	{
		std::swap(Node::nodeState.state[row_zero_pos][col_zero_pos],
							nodeState.state[row_zero_pos][col_zero_pos - 1]);
	}
}

bool Node::IsGoalState(uint8_t goalState[][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (nodeState.state[i][j] != goalState[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

void Node::FindZeroPos() //returns index of zero pos.
{
	for (int row = 0; row < 4; ++row) //row, so essentially acts as y-coordinate
	{
		for (int col = 0; col < 4; ++col) //column so acts as x-coordinate
		{
			if (nodeState.state[row][col] == 0)
			{
				//flipping these around is wrong; must look into this.
				row_zero_pos = row; //x-coordinate
				col_zero_pos = col; //y-coordinate
				return;
			}
		}
	}
}

uint8_t Node::GetRowZeroPos()
{
	return row_zero_pos;
}

uint8_t Node::GetColZeroPos()
{
	return col_zero_pos;
}