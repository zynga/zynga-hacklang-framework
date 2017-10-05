<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Fields;

use Zynga\Framework\Exception\V1\Exception;

class FieldsGetForObject extends Fields {
  public function getForObject(): Map<string, mixed> {
    throw new Exception('Broken Get For Object');
  }
}
