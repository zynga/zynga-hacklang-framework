<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Interfaces;

interface ImportInterface {
  public function fromVector(Vector<mixed> $data): bool;
  public function fromMap(Map<string, mixed> $data): bool;
  public function fromJSON(string $payload): bool;
  public function fromBinary(string $payload): bool;
}
