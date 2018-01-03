<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\StorableObject\V1\StorableVector;
use Zynga\Framework\Type\V1\StringBox;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButHasConstructorArgs
;

class ValidNestedVectorButBrokenChild extends StorableObjectBase {
  public StorableVector<StringBox> $stringVec;
  public StorableVector<ValidButHasConstructorArgs> $validVec;

  public function __construct() {

    parent::__construct();

    $this->stringVec = new StorableVector();
    $this->validVec = new StorableVector();

  }

}
