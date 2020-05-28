<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

enum PgReaderWriterBehavior: int {
  Default = 0;
  Fails = 1;
  ThrowsException = 2;
}
