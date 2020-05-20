<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

enum SaveBehavior: int {
  Succeeds = 0;
  Fails = 1;
  ThrowsException = 2;
}
