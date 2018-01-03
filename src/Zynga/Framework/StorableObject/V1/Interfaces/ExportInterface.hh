<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Interfaces;

interface ExportInterface {
  public function asJSON(
    ?string $parentFieldName = null,
    bool $sorted = false,
  ): string;
  public function asMap(): Map<string, mixed>;
  public function asBinary(): string;
}
