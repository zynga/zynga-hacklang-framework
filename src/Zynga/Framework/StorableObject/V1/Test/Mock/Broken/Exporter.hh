<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Object\Exporter as ExporterBase;

use Zynga\Framework\Exception\V1\Exception;

class Exporter implements ExportInterface {

  public function __construct(StorableObjectInterface $obj) {}

  public function asJSON(?string $parentFieldName = null, bool $sorted = false): string {
    throw new Exception('broken-as-map');
  }
  public function asMap(): Map<string, mixed> {
    throw new Exception('broken-as-map');
  }
  public function asBinary(): string {
    throw new Exception('broken-as-binary');
  }
}
