#ifndef CHESS_CONSTANTS_H
#define CHESS_CONSTANTS_H

#include <cstdint>

namespace Constant {
  constexpr std::uint64_t RANK_1 = 0x00000000000000FFULL;
  constexpr std::uint64_t RANK_2 = 0x000000000000FF00ULL;
  constexpr std::uint64_t RANK_3 = 0x0000000000FF0000ULL;
  constexpr std::uint64_t RANK_4 = 0x00000000FF000000ULL;
  constexpr std::uint64_t RANK_5 = 0x000000FF00000000ULL;
  constexpr std::uint64_t RANK_6 = 0x0000FF0000000000ULL;
  constexpr std::uint64_t RANK_7 = 0x00FF000000000000ULL;
  constexpr std::uint64_t RANK_8 = 0xFF00000000000000ULL;

  constexpr std::uint64_t FILE_A = 0x0101010101010101ULL;
  constexpr std::uint64_t FILE_B = 0x0202020202020202ULL;
  constexpr std::uint64_t FILE_C = 0x0404040404040404ULL;
  constexpr std::uint64_t FILE_D = 0x0808080808080808ULL;
  constexpr std::uint64_t FILE_E = 0x1010101010101010ULL;
  constexpr std::uint64_t FILE_F = 0x2020202020202020ULL;
  constexpr std::uint64_t FILE_G = 0x4040404040404040ULL;
  constexpr std::uint64_t FILE_H = 0x8080808080808080ULL;
}

#endif
