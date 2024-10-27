#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/board.h"
#include <cstdint>
#include <ostream>
#include <sys/types.h> 

#include <iostream>
#include <string>
#include <tuple>

uint64_t Board::fullBoard() {
  return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
          blackKing) |
         (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
          whiteKing);
}

void Board::printBitBoard(uint64_t bitBoard) {
  std::string row = "";

  for (int i = 63; i >= 0; i--) {
    row = " " + std::to_string((bitBoard >> i) & 1) + row;
    if (i % 8 == 0) {
      std::cout << row << std::endl;
      row = "";
    }
  }
}

void Board::printBoard() { printBitBoard(fullBoard()); }

uint64_t Board::enemySquares() {
  if (wTurn) {
    return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
            blackKing);
  }
  return (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
          whiteKing);
}

uint64_t Board::allySquares() {
  if (wTurn) {
    return (whitePawn | whiteKnight | whiteBishop | whiteRook | whiteQueen |
            whiteKing);
  }
  return (blackPawn | blackKnight | blackBishop | blackRook | blackQueen |
          blackKing);
}

uint64_t Board::pawnAttacks(uint64_t piece = (uint64_t) -1) {
  piece = (piece == (uint64_t) -1) ? (wTurn) ? whitePawn : blackPawn : piece;
  uint64_t attacks;
  if (wTurn) {
    attacks = (piece & ~Constant::FILE_A) << 7 |
              (piece & ~Constant::FILE_H) << 9;
  } else {
    attacks = (piece & ~Constant::FILE_H) >> 7 |
              (piece & ~Constant::FILE_A) >> 9;
  }

  return attacks;
}

uint64_t Board::pawnMoves(uint64_t piece) {
  uint64_t attacks;
  uint64_t moves;

  if (wTurn) {
    moves = (piece & Constant::RANK_2) << 16 | whitePawn << 8;
  } else {
    moves = (piece & Constant::RANK_7) >> 16 | blackPawn >> 8;
  }

  attacks = pawnAttacks(piece) & enemySquares();
  moves &= (~allySquares() | ~enemySquares());

  // implement en passant :(
  // use a function to keep track of any double push up pawns

  return attacks | moves;
  
}
uint64_t Board::pawnMoveGen() {
  if (wTurn) {
    return pawnMoves(whitePawn);
  }
  return pawnMoves(blackPawn);
}

uint64_t Board::knightMoveGen() {
  uint64_t moves;
  if (wTurn) {
    moves = knightMoves(whiteKnight);
  }
  else {
    moves = knightMoves(blackKnight);
  }
  moves &= ~allySquares();

  return moves;
}

uint64_t Board::knightMoves(uint64_t piece) {
  uint64_t moves;

  moves = (piece << 17) & ~Constant::FILE_A;
  moves |= ((piece << 10) & ~(Constant::FILE_A | Constant::FILE_B));
  moves |= ((piece >> 6) & ~(Constant::FILE_A | Constant::FILE_B));
  moves |= ((piece >> 15) & ~Constant::FILE_A);

  moves |= ((piece << 15) & ~Constant::FILE_H);
  moves |= ((piece << 6) & ~(Constant::FILE_G | Constant::FILE_H));
  moves |= ((piece >> 10) & ~(Constant::FILE_G | Constant::FILE_H));
  moves |= ((piece >> 17) & ~Constant::FILE_H);


  return moves;
}

uint64_t Board::attackedSquares() {
  wTurn = !wTurn;
  // add other pieces
  uint64_t attacked = pawnAttacks() | knightMoveGen() | kingMoveGen() | bishopMoveGen() | rookMoveGen() | queenMoveGen();
  wTurn = !wTurn;
  return attacked;
}

uint64_t Board::handleCastle() {
  uint64_t moves;

  return moves;
}

uint64_t Board::kingMoveGen() {
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

uint64_t Board::diagMoveGen(uint64_t piece) {
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

uint64_t Board::bishopMoveGen() {
  uint64_t piece = (wTurn) ? whiteBishop : blackBishop;
  
  return diagMoveGen(piece);
}

uint64_t Board::straightMoveGen(uint64_t piece) {
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

uint64_t Board::rookMoveGen() {
  uint64_t piece = (wTurn) ? whiteRook : blackRook;

  return straightMoveGen(piece);
}

uint64_t Board::queenMoveGen() {
  uint64_t piece = (wTurn) ? whiteQueen : blackQueen;

  return diagMoveGen(piece) | straightMoveGen(piece);
}

std::tuple<uint64_t, uint64_t> Board::moveGen(int identifier, uint64_t piece) {
  switch (identifier) {
    case Piece::King:
      return {kingMoveGen(), 1};
    case Piece::Queen:
      return {queenMoveGen(), diagMoveGen(piece) | straightMoveGen(piece)};
    case Piece::Rook:
      return {rookMoveGen(), straightMoveGen(piece)};
    case Piece::Bishop:
      return {bishopMoveGen(), diagMoveGen(piece)};
    case Piece::Knight:
      return {knightMoveGen(), knightMoves(piece)};
    case Piece::Pawn:
      return {pawnMoveGen(), pawnMoves(piece)};
  }

  /*std::cout << "passing";*/
  return {-1, -1};
}

std::tuple<int, uint64_t> Board::algebraicNotation(std::string move) {
  auto len = move.length();

  switch (len) {
    case 2: {
      return {Piece::Pawn, matchFile(move[len - 2]) & matchRank(move[len - 1])};
    }
    case 3: {
      return {move[0], matchFile(move[len - 2]) & matchRank(move[len - 1])};
    }
  }
  return {-1, -1};
}

// going to have to separate the sliding move gen functions into their own directions to xor and remove
// for knight, ~ the colorKnight then ^ with move gen of the algebraicNotation move ~ again then ^ with self
// movegen on algebraic move -> & with piece -> ^ to remove piece from updated position.
// for pawn, i think something similar would work?
// generalize your movegen so you can use it!!!
void Board::makeMove(std::string notation) {
  auto [identifier, move] = algebraicNotation(notation);
  std::cout << identifier << std::endl;
  std::cout << Piece::Knight << std::endl;
  auto [possible_moves, backtracked_moves] = moveGen(identifier, move);

  if (possible_moves == (uint64_t) -1 || ! (move & possible_moves)) {
    return;
  }

  uint64_t* piece = nullptr;
  if (wTurn) {
    piece = pieceMapW.at(identifier);
  }
  else {
    piece = pieceMapB.at(identifier);
  }

  *piece ^= (*(piece) & backtracked_moves);
  
  std::cout << "\ntest" << std::endl;
  printBitBoard((backtracked_moves));
  std::cout << "test\n" << std::endl;

  *piece |= move;

  return;
}

int main() {
  Board board = Board();

  /*board.printBoard();*/

  /*board.printBitBoard(board.pawnMoveGen());*/
  board.makeMove("Nc3");

  board.printBoard();

  /*auto [ piece, move ] = board.algebraicNotation("e4");*/
  /*board.printBitBoard(move);*/
}