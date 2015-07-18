#ifndef Pawn_H
#define Pawn_H


#include "Board.hpp"
#include "Color.hpp"
#include "BitBoardUtils.hpp"

namespace Pawn
{
	const static unsigned int DOUBLED_PAWN_PENALTY = -20;
	const static unsigned int ISOLATED_PAWN_PENALTY = -10;
	const static unsigned int PASSED_PAWN_BONUS = 15;

	/** Score calculation **/
	int initScore(const Board &board);
    int countPawnsInFile(const Board &board, const int file, const int color);
	int initDoubledPawns(const Board &board);
	int initPassedPawns(const Board &board);
	int initIsolatedPawn(const Board &board);

	/** Utils **/
	int countPawns(const Board &board, const int color);
    bool hasNeighbors(const Board &board, const int file, const int color);

};

#endif // Pawn_H
