#include "catch.hpp"
#include "Board.hpp"
#include "Position.hpp"
#include "Rook.hpp"
#include "Color.hpp"
#include "Piece.hpp"

TEST_CASE( "is position on Board test", "[board]" )
{
    Board board;
    Position position1(0, 0);
    Position position2(7,7);
    Position position3(6,4);

    Position position4(8,4);
    Position position5(4,8);
    Position position6(-1,4);
    Position position7(6,-1);

    REQUIRE(board.isPositionOnBoard(position1));
    REQUIRE(board.isPositionOnBoard(position2));
    REQUIRE(board.isPositionOnBoard(position3));

    REQUIRE_FALSE(board.isPositionOnBoard(position4));
    REQUIRE_FALSE(board.isPositionOnBoard(position5));
    REQUIRE_FALSE(board.isPositionOnBoard(position6));
    REQUIRE_FALSE(board.isPositionOnBoard(position7));
}

TEST_CASE( "is position free", "[board]" )
{
    Board board;

    Position position1(0, 0);
    Position position2(7,7);
    Position position3(6,4);

	PiecePtr rook1(new Rook(position1, WHITE));
	PiecePtr rook2(new Rook(position2, WHITE));
	PiecePtr rook3(new Rook(position3, WHITE));

    board.addPiece(rook1);
    board.addPiece(rook2);
    board.addPiece(rook3);

    Position position4(5,4);
    Position position5(4,6);
    Position position6(5,7);
    Position position7(4,0);

    REQUIRE_FALSE(board.isPositionFree(position1));
    REQUIRE_FALSE(board.isPositionFree(position2));
    REQUIRE_FALSE(board.isPositionFree(position3));

    REQUIRE(board.isPositionFree(position5));
    REQUIRE(board.isPositionFree(position4));
    REQUIRE(board.isPositionFree(position6));
    REQUIRE(board.isPositionFree(position7));
}

TEST_CASE( "getPieces", "[board]" )
{
    Board board;

    Position position1(0, 0);
    Position position2(7,7);
    Position position3(6,4);

	PiecePtr rook1(new Rook(position1, WHITE));
	PiecePtr rook2(new Rook(position2, BLACK));
	PiecePtr rook3(new Rook(position3, WHITE));

    board.addPiece(rook1);
    board.addPiece(rook2);
    board.addPiece(rook3);

	int numberWhitePieces = board.getPieces(WHITE).size();
	int numberBlackPieces = board.getPieces(BLACK).size();

    REQUIRE(numberWhitePieces == 2);
    REQUIRE(numberBlackPieces == 1);
}
