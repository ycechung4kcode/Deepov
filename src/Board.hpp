/*
 * Board.hpp
 *
 *  Created on: 4 oct. 2014
 *      Author: Romain & Navid
 */
#ifndef Board_HPP_
#define Board_HPP_

#include <cstdint>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <memory>
#include <array>


#ifndef _U64
typedef std::uint64_t U64;
#define _U64
#endif // _U64

#include "Move.hpp"
#include "Types.hpp"
#include "Tables.hpp"
#include "Piece.hpp"

/*
Note: We use Little-Endian Rank-File Mapping
The bitboard (U64) in binary representation is  H8-G8-F8-....-C1-B1-A1
Left shift '<<' means +1 on the chessboard
Right shift '>>' means -1 on the chessboard
The compass is:
    +7  +8  +9
    -1   0  +1
    -9  -8  -7
*/

class Board : public std::enable_shared_from_this<Board>
{
public:

	Board();
	Board(const std::string fen);

	/* BitBoard getters */
	inline U64 getWhitePawns() const{return myBitboards[0];}
	inline U64 getWhiteKnights() const{return myBitboards[1];}
	inline U64 getWhiteBishops() const{return myBitboards[2];}
	inline U64 getWhiteRooks() const{return myBitboards[3];}
	inline U64 getWhiteQueens() const{return myBitboards[4];}
	inline U64 getWhiteKing() const{return myBitboards[5];}

	inline U64 getBlackPawns() const{return myBitboards[6];}
	inline U64 getBlackKnights() const{return myBitboards[7];}
	inline U64 getBlackBishops() const{return myBitboards[8];}
	inline U64 getBlackRooks() const{return myBitboards[9];}
	inline U64 getBlackQueens() const{return myBitboards[10];}
	inline U64 getBlackKing() const{return myBitboards[11];}

	inline U64 getPawns(Color color) const{return getBitBoard(Piece::PAWN, color);}
	inline U64 getKnights(Color color) const{return getBitBoard(Piece::KNIGHT, color);}
	inline U64 getBishops(Color color) const{return getBitBoard(Piece::BISHOP, color);}
	inline U64 getRooks(Color color) const{return getBitBoard(Piece::ROOK, color);}
	inline U64 getQueens(Color color) const{return getBitBoard(Piece::QUEEN, color);}
	inline U64 getKing(Color color) const{return getBitBoard(Piece::KING, color);}

	inline U64 getAllPawns() const{return myBitboards[14];}
	inline U64 getAllKnights() const{return myBitboards[15];}
	inline U64 getAllBishops() const{return myBitboards[16];}
	inline U64 getAllRooks() const{return myBitboards[17];}
	inline U64 getAllQueens() const{return myBitboards[18];}
	inline U64 getAllKings() const{return myBitboards[19];}

	inline U64 getWhitePieces() const{return myBitboards[12];}
	inline U64 getBlackPieces() const{return myBitboards[13];}
	inline U64 getAllPieces() const{return myAllPieces;}
	inline U64 getPieces(const Color color) const {return myBitboards[12+color];}
	inline U64 getPinnedPieces() const{return myPinnedPieces;}

	inline U64 getBitBoard(Piece::PieceType pieceType, Color color) const{return myBitboards[pieceType+6*color];};

	inline U64 getAtkFr(Square sq) const {return myAtkFr[sq];};
	inline U64 getAtkTo(Square sq) const {return myAtkTo[sq];};


	inline Color getColorToPlay() const {return myColorToPlay;};
	inline std::vector<Move> getMovesHistory() const {return myMoves;};

	/*  **********  */

	void setBitBoards(const std::string piecesString, const unsigned int rank);
	void updateConvenienceBitboards();
    void updatePinnedPieces();
    void updateAtkFr();

	/* Moves methods */
    bool isMoveLegal(Move &move, bool isCheck); // uses executeMove and undoMove so it can't be const
    void executeMove(Move &move);
    void undoMove(Move &move);
    unsigned int perft(unsigned int depth);
    unsigned int divide(unsigned int depth);
    boost::optional<Move> getEnemyLastMove() const; // I use boost::optional in case there is no move to return

    //Castling Methods
    inline bool isQueenSideCastlingAllowed(const Color color) const {return (myCastling >> (1+2*color)) & 0x1;}
    inline bool isKingSideCastlingAllowed(const Color color) const {return (myCastling >> 2*color) & 0x1;}
    void updateCastlingRights(Move &move);
    void rewindCastlingRights(const Move &move);


    //PieceType method
    Piece::Piece findPieceType(const Square position) const;
    Piece::PieceType findPieceType(const Square position, const Color color) const;
    Piece::PieceType findWhitePieceType(const Square position) const;
    Piece::PieceType findBlackPieceType(const Square position) const;

    //Check methods
    bool isCheck(const Color color) const;

    //Attacked positions
    U64 getAttacksFromSq(const Square position) const;

    inline U64 getKnightDestinations(const Square pos, const Color color) const
    {
        return Tables::ATTACK_TABLE[Piece::KNIGHT][pos] & ~getPieces(color);
    };

    inline U64 getPawnAttackedDestinations(const Square square) const
    {
        return Tables::PAWN_ATTACK_TABLE[myColorToPlay][square];
    };

    inline U64 getKnightAttackedDestinations(const Square square) const
    {
        return Tables::ATTACK_TABLE[Piece::KNIGHT][square];
    };

    inline U64 getBishopAttackedDestinations(const Square square) const
    {
        return MagicMoves::Bmagic(square, getAllPieces());
    };

    inline U64 getRookAttackedDestinations(const Square square) const
    {
        return MagicMoves::Rmagic(square, getAllPieces());
    };

    inline U64 getQueenAttackedDestinations(const Square square) const
    {
        return getBishopAttackedDestinations(square) | getRookAttackedDestinations(square);
    };

    inline U64 getKingDestinations(const Square square, const Color color) const
    {
        return Tables::ATTACK_TABLE[Piece::KING][square] & ~getPieces(color);
    };

    U64 getAttackedPositions(const Color color) const;
    U64 getKingAttackedPositions(const Color color) const;

    U64 getQueenAttackedPositions(const Color color) const;
    U64 getRookAttackedPositions(const Color color) const;
    U64 getBishopAttackedPositions(const Color color) const;
    U64 getKnightAttackedPositions(const Color color) const;
    U64 getPawnAttackedPositions(const Color color) const;

	/* ************* */
    char getChar(const unsigned int file, const unsigned int rank) const;

    /* This method takes a position as a string (e.g. e4) and returns the index in Little-Endian
    It will replace Utils::getPosition(), this version should be much better.
    In our board model row starts at index 0. a will be 0 and h will be 7 */
    inline unsigned int getIndexFromChar(const std::string position) const {return(position[0] - 'a'+8*(position[1] - '0'-1));};


private:
    std::array<U64, 20> myBitboards;
	U64 myAllPieces;
	U64 myPinnedPieces;

	Color myColorToPlay;
    unsigned int myCastling; /* Same order as FEN
    RIGHT BIT : white king side, white queen side, black king side, black queen side : LEFT BIT
    e.g. the order is 1111 = qkQK */

    U64 myAtkTo[SQUARE_NB]; // Locations of pieces that attack to the square
    U64 myAtkFr[SQUARE_NB]; // Attacks from the piece on the square

    unsigned int myMovesCounter;
    unsigned int myHalfMovesCounter;

    std::vector<Move> myMoves;

    //Move methods
    inline void movePiece(const Square origin, const Square destination, const unsigned int pieceType, const Color color)
    {
    	movePiece(origin, destination, myBitboards[pieceType+color*6]);
    }

    inline void removePiece(const Square index, const unsigned int pieceType, const Color color)
    {
    	removePiece(index, myBitboards[pieceType+color*6]);
    }

    inline void addPiece(const Square index, const unsigned int pieceType, const Color color)
    {
    	addPiece(index, myBitboards[pieceType+color*6]);
    }
    inline void removePiece(const Square index, U64 &bitBoard)
    {
    	bitBoard &= ~(1LL << index);
    }

    inline void addPiece(const Square index, U64 &bitBoard)
    {
    	bitBoard |=  1LL << index;
    }

    inline void movePiece(const Square origin, const Square destination, U64 &bitBoard)
    {
    	//Remove piece from origin position
    	removePiece(origin, bitBoard);
    	//Add piece to destination positions
    	addPiece(destination, bitBoard);
   }
};


inline std::ostream& operator<<(std::ostream &strm, const Board &Board) {

	for(int rank = 7; rank >= 0 ; rank--)
	{
		strm << rank << "|  ";

		for(int file = 0; file < 8 ; file++)
		{
			strm << Board.getChar(file, rank) << " ";
		}

		strm << std::endl;
	}

	strm << "   ________________" << std::endl;
	strm << "    0 1 2 3 4 5 6 7" << std::endl;

	return strm;
}

#endif /* Board_HPP_ */
