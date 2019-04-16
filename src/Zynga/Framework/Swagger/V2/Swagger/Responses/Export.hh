<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger\Responses;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;

use Zynga\Framework\Swagger\V2\Swagger\Responses as SwaggerResponses;

class Export implements ExportInterface {
  private SwaggerResponses $_object;

  public function __construct(SwaggerResponses $object) {
    $this->_object = $object;
  }

  public function asJSON(?string $parentFieldName = null, bool $sorted = false): string {
    $data = '';

    if ($parentFieldName !== null) {
      $data .= json_encode($parentFieldName).':';
    }

    $data .= '{';

    $responses = $this->_object->getMap();

    $firstItem = true;

    foreach ($responses as $http_code => $responseObject) {
      if ($responseObject instanceof StorableObjectInterface) {
        if ($firstItem === false) {
          $data .= ',';
        }
        $data .= '"'.$http_code.'": ';
        $data .= $responseObject->export()->asJSON(null, $sorted);
        $firstItem = false;
      }
    }

    $data .= '}';
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
