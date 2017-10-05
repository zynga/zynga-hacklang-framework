<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\Fields;

class ValidButBrokenFields extends Valid {

  public function fields(): FieldsInterface {
    $fields = new Fields($this);
    return $fields;
  }

}
