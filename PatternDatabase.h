#pragma once
#include<cstdint>
#include<set>
#include<queue>
#include"Node.h"
#define NUM_PIECES 3
#define BOARD_SIZE 16

struct PieceIndex
{
	uint8_t index[2]; //2-D array; row, and then col.
};

class PatternDatabase
{
	public:

		struct Position: Node::State //if we want, we can find the indexes (instead of searching it from board)
		{
			int cost; //store cost from goal to the position of the piece index.
			uint8_t pieceValue[NUM_PIECES]; //value of the piece itself
			PieceIndex pieceCoordinate[NUM_PIECES];
		};

		std::set<Position> patternDB; //keep the patterns in memory - with 2 pieces, there will be around 2,000 positions

		Position position;

		 //call for each time we reset the pieces
		//check if a move is fine, so long as the move you're swapping to is not a pieceOne, or pieceTwo.

		void bfs(uint64_t& nodesExpanded, PatternDatabase::Position& initialPos);

		void GetIndexOfTile(int& pieceIndex);

		bool IsValidMove(int& move, int& pieceIndex);

		void SetPositionFromNode(Node& node);

		void SetPosition();

		void BuildPatterns(uint64_t& nodesExpanded);

		void MakeMove(int& move, int& pieceIndex);

	private:
		uint8_t goalState[4][4] = {{1, 2, 3, 4},
						   {5, 6, 7, 8},
						   {9, 10, 11, 12},
						   {13, 14, 15, 0}};

};
