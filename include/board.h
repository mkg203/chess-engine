#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "utils.h"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h> 
#include <unordered_map>
#include <tuple>

class Board {
private:
  uint64_t whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen,
      whiteKing;
  uint64_t blackPawn, blackKnight, blackBishop, blackRook, blackQueen,
      blackKing;
  bool wTurn;

  std::unordered_map<int, uint64_t *> pieceMapW = {
    {Piece::King, &whiteKing},
    {Piece::Queen, &whiteQueen},
    {Piece::Rook, &whiteRook},
    {Piece::Bishop, &whiteBishop},
    {Piece::Knight, &whiteKnight},
    {Piece::Pawn, &whitePawn},
  };

  std::unordered_map<int, uint64_t *> pieceMapB = {
    {Piece::King, &blackKing},
    {Piece::Queen, &blackQueen},
    {Piece::Rook, &blackRook},
    {Piece::Bishop, &blackBishop},
    {Piece::Knight, &blackKnight},
    {Piece::Pawn, &blackPawn},
  };

public:
  Board() {
    wTurn = true;

    blackPawn = 0b11111111;
    blackPawn <<= 48;

    blackKnight = 0b01000010;
    blackKnight <<= 56;

    blackBishop = 0b00100100;
    blackBishop <<= 56;

    blackRook = 0b10000001;
    blackRook <<= 56;

    blackQueen = 0b00010000;
    blackQueen <<= 56;

    blackKing = 0b00001000;
    blackKing <<= 56;

    whitePawn = 0b11111111;
    whitePawn <<= 8;

    whiteKnight = 0b01000010;

    whiteBishop = 0b00100100;

    whiteRook = 0b10000001;

    whiteQueen = 0b00001000;

    whiteKing = 0b00010000;
  }
  
  uint64_t fullBoard();

  static void printBitBoard(uint64_t bitBoard);

  void printBoard();

  uint64_t enemySquares();

  uint64_t allySquares();

  uint64_t pawnAttacks(uint64_t);

  uint64_t pawnMoves(uint64_t);

  uint64_t pawnMoveGen();

  uint64_t knightMoveGen();

  uint64_t knightMoves(uint64_t);
  
  uint64_t attackedSquares();

  uint64_t handleCastle();
  
  uint64_t kingMoveGen();

  uint64_t diagMoveGen(uint64_t, bool);

  uint64_t straightMoveGen(uint64_t, bool);
  
  uint64_t bishopMoveGen();
  
  uint64_t rookMoveGen();
  
  uint64_t queenMoveGen();
  
  std::tuple<uint64_t, uint64_t> moveGen(int, uint64_t);
  
  std::tuple<int, uint64_t> algebraicNotation(std::string);
  
  void makeMove(std::string);

  uint64_t backtrackPawnMove(uint64_t);
};

#endif
