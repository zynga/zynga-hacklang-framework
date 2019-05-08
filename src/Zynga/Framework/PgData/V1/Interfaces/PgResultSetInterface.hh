<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

interface PgResultSetInterface {
  public function createChecksum(): string;
}
