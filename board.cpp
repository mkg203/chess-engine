#include "constants.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>

class Board {
private:
  uint64_t whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen,
      whiteKing;
  uint64_t blackPawn, blackKnight, blackBishop, blackRook, blackQueen,
      blackKing;
  bool wTurn;

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

    whiteQueen = 0b00010000;

    whiteKing = 0b00001000;
  }

  uint64_t fullBoard() {
    return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
            blackKing) |
           (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
            whiteKing);
  }

  static void printBitBoard(uint64_t bitBoard) {
    std::string row = "";

    for (int i = 63; i >= 0; i--) {
      row = " " + std::to_string((bitBoard >> i) & 1) + row;
      if (i % 8 == 0) {
        std::cout << row << std::endl;
        row = "";
      }
    }
  }

  void printBoard() { printBitBoard(fullBoard()); }

  uint64_t enemySquares() {
    if (wTurn) {
      return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
              blackKing);
    }
    return (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
            whiteKing);
  }

  uint64_t allySquares() {
    if (wTurn) {
      return (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
              whiteKing);
    }
    return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
            blackKing);
  }

  uint64_t pawnAttacks() {
    uint64_t attacks;
    if (wTurn) {
      attacks = (whitePawn & ~Constant::FILE_A) << 7 |
                (whitePawn & ~Constant::FILE_H) << 9;
    } else {
      attacks = (blackPawn & ~Constant::FILE_H) >> 7 |
                (blackPawn & ~Constant::FILE_A) >> 9;
    }

    return attacks;
  }
  uint64_t pawnMoveGen() {
    uint64_t attacks;
    uint64_t moves;

    if (wTurn) {
      moves = (whitePawn & Constant::RANK_2) << 16 | whitePawn << 8;
    } else {
      moves = (blackPawn & Constant::RANK_7) >> 16 | blackPawn >> 8;
    }

    attacks = pawnAttacks() & enemySquares();
    moves &= (~allySquares() | ~enemySquares());

    // impement en passant :(
    // use a function to keep track of any double push up pawns

    return attacks | moves;
  }

  uint64_t knightMoveGen() {
    uint64_t moves;

    if (wTurn) {
      moves = (whiteKnight << 17) & ~Constant::FILE_A;
      moves |= ((whiteKnight << 10) & ~(Constant::FILE_A | Constant::FILE_B));
      moves |= ((whiteKnight >> 6) & ~(Constant::FILE_A | Constant::FILE_B));
      moves |= ((whiteKnight >> 15) & ~Constant::FILE_A);

      moves |= ((whiteKnight << 15) & ~Constant::FILE_H);
      moves |= ((whiteKnight << 6) & ~(Constant::FILE_G | Constant::FILE_H));
      moves |= ((whiteKnight >> 10) & ~(Constant::FILE_G | Constant::FILE_H));
      moves |= ((whiteKnight >> 17) & ~Constant::FILE_H);
    } else {
      moves = (blackKnight << 17) & ~Constant::FILE_A;
      moves |= ((blackKnight << 10) & ~(Constant::FILE_A | Constant::FILE_B));
      moves |= ((blackKnight >> 6) & ~(Constant::FILE_A | Constant::FILE_B));
      moves |= ((blackKnight >> 15) & ~Constant::FILE_A);

      moves |= ((blackKnight << 15) & ~Constant::FILE_H);
      moves |= ((blackKnight << 6) & ~(Constant::FILE_G | Constant::FILE_H));
      moves |= ((blackKnight >> 10) & ~(Constant::FILE_G | Constant::FILE_H));
      moves |= ((blackKnight >> 17) & ~Constant::FILE_H);
    }

    moves &= ~allySquares();

    return moves;
  }

  uint64_t attackedSquares() {
    wTurn = !wTurn;
    // add other pieces
    uint64_t attacked = pawnAttacks() | knightMoveGen() | kingMoveGen();
    wTurn = !wTurn;
    return attacked;
  }

  uint64_t kingMoveGen() {
    uint64_t moves;
    if (wTurn) {
      moves = whiteKing << 8;
      moves |= whiteKing >> 8;
      moves |= (whiteKing << 1) & ~Constant::FILE_H;
      moves |= (whiteKing >> 1) & ~Constant::FILE_A;
    } else {
      moves = blackKing << 8;
      moves |= blackKing >> 8;
      moves |= (blackKing << 1) & ~Constant::FILE_H;
      moves |= (blackKing >> 1) & ~Constant::FILE_A;
    }

    moves &= allySquares();
    moves &= ~attackedSquares();

    return moves;
  }

  uint64_t bishopMoveGen() {
    uint64_t piece = (wTurn) ? whiteBishop : blackBishop;
    uint64_t moves_nw = piece;
    uint64_t moves_ne = piece;
    uint64_t moves_sw = piece;
    uint64_t moves_se = piece;

    for (int i = 0; i < 8; i++) {
      moves_ne |= ((allySquares() | enemySquares()) ^
                   ((moves_ne << 9) & ~Constant::FILE_A)) &
                  ((moves_ne << 9) & ~Constant::FILE_A);
      moves_nw |= ((allySquares() | enemySquares()) ^
                   ((moves_nw << 7) & ~Constant::FILE_H)) &
                  ((moves_nw << 7) & ~Constant::FILE_H);

      moves_sw |= ((allySquares() | enemySquares()) ^
                   ((moves_sw >> 9) & ~Constant::FILE_H)) &
                  ((moves_sw >> 9) & ~Constant::FILE_H);
      moves_se |= ((allySquares() | enemySquares()) ^
                   ((moves_se >> 7) & ~Constant::FILE_A)) &
                  ((moves_se >> 7) & ~Constant::FILE_A);
    }

    moves_ne |= (enemySquares() & ((moves_ne << 9) & ~Constant::FILE_A));
    moves_nw |= (enemySquares() & ((moves_nw << 7) & ~Constant::FILE_H));

    moves_sw |= (enemySquares() & ((moves_sw >> 9) & ~Constant::FILE_H));
    moves_se |= (enemySquares() & ((moves_se >> 7) & ~Constant::FILE_A));

    return (moves_nw | moves_ne | moves_se | moves_sw);
  }

  uint64_t rookMoveGen() {
    uint64_t piece = (wTurn) ? whiteRook : blackRook;
    uint64_t moves_n = piece;
    uint64_t moves_s = piece;
    uint64_t moves_e = piece;
    uint64_t moves_w = piece;

    for (int i = 0; i < 8; i++) {
      moves_n |=
          ((allySquares() | enemySquares()) ^ (moves_n << 8)) & (moves_n << 8);
      moves_s |=
          ((allySquares() | enemySquares()) ^ (moves_s >> 8)) & (moves_s >> 8);

      moves_e |= ((allySquares() | enemySquares()) ^
                  ((moves_e >> 1) & ~Constant::FILE_H)) &
                 ((moves_e >> 1) & ~Constant::FILE_H);
      moves_w |= ((allySquares() | enemySquares()) ^
                  ((moves_w << 1) & ~Constant::FILE_A)) &
                 ((moves_w << 1) & ~Constant::FILE_A);
    }

    moves_n |= (enemySquares() & (moves_n << 8));
    moves_s |= (enemySquares() & (moves_n >> 8));
    moves_e |= (enemySquares() & (moves_e >> 1) & ~Constant::FILE_H);
    moves_w |= (enemySquares() & (moves_e << 1) & ~Constant::FILE_A);

    return (moves_n | moves_e | moves_s | moves_w);
  }

  uint64_t queenMoveGen() {
    uint64_t moves;

    return moves;
  }
};

int main() {
  Board board = Board();

  /*board.printBoard();*/

  board.printBitBoard(board.bishopMoveGen());
}
