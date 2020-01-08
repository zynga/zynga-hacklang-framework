<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;

use Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

class Export implements ExportInterface {
  private DefinitionReference $_object;

  public function __construct(DefinitionReference $object) {
    $this->_object = $object;
  }

  public function asJSON(?string $parentFieldName = null, bool $sorted = false): string {

    $data = '';

    list($isDefaultValue, $defaultValues) = $this->_object->isDefaultValue();

    if ($isDefaultValue === true) {
      return $data;
    }

    $value = $this->_object->get();

    $className = strval($this->_object->get());

    if ($className == '') {
      return $data;
    }

    if ($parentFieldName !== null) {
      $data .= json_encode($parentFieldName).':';
    }

    $data = '"$ref":"#/definitions/'.$className.'"';

    return $data;

  }

  public function asArray(): array<string, mixed> {
    $map = array();
    return $map;
  }

  public function asMap(): Map<string, mixed> {
    $map = Map {};
    return $map;
  }

  public function asBinary(): string {
    return '';
  }

}
