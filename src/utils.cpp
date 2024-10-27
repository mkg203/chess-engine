#include "../include/constants.h"


uint64_t matchFile(char file) {
  switch (file) {
    case 'a': return Constant::FILE_A;
    case 'b': return Constant::FILE_B;
    case 'c': return Constant::FILE_C;
    case 'd': return Constant::FILE_D;
    case 'e': return Constant::FILE_E;
    case 'f': return Constant::FILE_F;
    case 'g': return Constant::FILE_G;
    case 'h': return Constant::FILE_H;
  }
  return 1;
}

uint64_t matchRank(char file) {
  switch (file) {
    case '1': return Constant::RANK_1;
    case '2': return Constant::RANK_2;
    case '3': return Constant::RANK_3;
    case '4': return Constant::RANK_4;
    case '5': return Constant::RANK_5;
    case '6': return Constant::RANK_6;
    case '7': return Constant::RANK_7;
    case '8': return Constant::RANK_8;
  }
  return 1;
}
