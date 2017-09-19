<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface;

interface ResultSetInterface {

  public function errorCapture(): ErrorCaptureInterface;

  public function wasSuccessful(): bool;

  public function hasCursor(): bool;
  public function freeCursor(): bool;

  public function setSql(string $sql): bool;
  public function getSql(): string;
  public function wasSqlDML(): bool;

  public function getNumRows(): int;
  public function rewind(int $pos): bool;

  public function hasMore(): bool;
  public function next(): bool;

  public function fetchMap(): Map<string, mixed>;
  public function fetchVector(): Vector<int>;

}
