<?hh // strict

namespace Zynga\Framework\PgData\V1;

enum PgWhereOperand : int {
  EQUALS = 2;
  NOT_EQUALS = 4;
  IN = 8;
  NOT_IN = 16;
  GREATER_THAN = 32;
  LESS_THAN = 64;

}

