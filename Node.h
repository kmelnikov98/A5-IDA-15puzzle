#pragma once
#include<cstdint>
#include<tuple>
#define NUM_MOVES 4

class Node
{
	public:
		enum Move
		{
			UP,
			RIGHT,
			DOWN,
			LEFT
		};

		struct State
		{
			uint8_t state[4][4] = {{1, 2, 3, 4},
								   {5, 6, 7, 8},
								   {9, 10, 11, 12},
								   {13, 14, 15, 0}};


			bool operator==(const State& rhsState) const
			{
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						if (state[i][j] != rhsState.state[i][j])
						{
							return false;
						}
					}
				}
				return true;
			}

			bool operator<(const State& rhsState) const
			{
				return std::tie(state[0][0], state[0][1], state[0][2], state[0][3],
					state[1][0], state[1][1], state[1][2], state[1][3],
					state[2][0], state[2][1], state[2][2], state[2][3],
					state[3][0], state[3][1], state[3][2], state[3][3])
					<
				std::tie(rhsState.state[0][0], rhsState.state[0][1], rhsState.state[0][2], rhsState.state[0][3],
						 rhsState.state[1][0], rhsState.state[1][1], rhsState.state[1][2], rhsState.state[1][3],
						 rhsState.state[2][0], rhsState.state[2][1], rhsState.state[2][2], rhsState.state[2][3],
						 rhsState.state[3][0], rhsState.state[3][1], rhsState.state[3][2], rhsState.state[3][3]);
			}
		};

		State nodeState;

		void FindZeroPos();

		void PrintState();

		void MakeMove(int& move); //move is 0 to 3 - UP, RIGHT, DOWN, LEFT

		bool IsValidMove(int& move);

		bool IsGoalState(uint8_t goalState[][4]);

		uint8_t GetRowZeroPos();

		uint8_t GetColZeroPos();

	private:
		uint8_t row_zero_pos; //gives row of zero position.
		uint8_t col_zero_pos; //gives column of zero position.

};

