<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

interface WriterInterface {
  public function add(PgRowInterface $row, bool $shouldUnlock): bool;
  public function addToDbOnly(PgRowInterface $row): bool;
  public function save(PgRowInterface $row, bool $shouldUnlock): bool;
  public function delete(PgRowInterface $obj, bool $shouldUnlock): bool;
}
