/*
 */

#include "Pawn.hpp"

int Pawn::initScore(const Board &board)
{
	return initDoubledPawns(board)+initPassedPawns(board)+initIsolatedPawn(board);
}

int Pawn::countPawnsInFile(const Board &board, const int file, const int color)
{
    U64 pawnsOnFile = board.getBitBoard(Piece::PAWN_TYPE,color) & LookUpTables::MASK_FILE[file];
    return BitBoardUtils::countBBBitsSet(pawnsOnFile);
}

int Pawn::initDoubledPawns(const Board &board)
{
	int whiteCount(0);
	int blackCount(0);
	int column;
	for (column=0; column<=7; column++)
	{
        int count=countPawnsInFile(board,column,WHITE);
		whiteCount += (count>1)*(count-1);
		count=countPawnsInFile(board,column,BLACK);
		blackCount += (count>1)*(count-1);
	}
	return (whiteCount-blackCount)*DOUBLED_PAWN_PENALTY;
}

int Pawn::initPassedPawns(const Board &board)
{
    int whiteCount(0);
	int blackCount(0);
	int column;

	for (column=0; column<=7; column++)
	{
        int countWhiteInFile=countPawnsInFile(board,column,WHITE);
        int countBlackInFile=countPawnsInFile(board,column,BLACK);
		whiteCount += (countWhiteInFile>0)&(countBlackInFile==0)&(!hasNeighbors(board,column,BLACK));
		blackCount += (countWhiteInFile==0)&(countBlackInFile>0)&(!hasNeighbors(board,column,WHITE));
	}
	return (whiteCount-blackCount)*PASSED_PAWN_BONUS;
}

int Pawn::initIsolatedPawn(const Board &board)
{
    int whiteCount(0);
	int blackCount(0);
	int column;

	for (column=0; column<=7; column++)
	{
        int count=countPawnsInFile(board,column,WHITE);
		whiteCount += (count>0)&(!hasNeighbors(board,column,WHITE));
		count=countPawnsInFile(board,column,BLACK);
		blackCount += (count>0)&(!hasNeighbors(board,column,BLACK));
	}

	return (whiteCount-blackCount)*ISOLATED_PAWN_PENALTY;
}

int Pawn::countPawns(const Board &board, const int color)
{
	return BitBoardUtils::countBBBitsSet(board.getPawns(color));
}

bool Pawn::hasNeighbors(const Board &board, const int file, const int color)
{
    return (board.getPawns(color) & LookUpTables::NEIGHBOR_FILES[file]);
}

