<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

interface StorableObjectInterface {

  public function fields(): FieldsInterface;
  public function import(): ImportInterface;
  public function export(): ExportInterface;

  public function setIsRequired(bool $isRequired): bool;
  public function getIsRequired(): bool;

  public function setIsDefaultValue(bool $tf): bool;
  public function isDefaultValue(): (bool, Vector<string>);

}
