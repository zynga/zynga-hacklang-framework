<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;

use Zynga\Framework\Type\V1\StringBox;

class ValidNested extends StorableObject {
  public StringBox $otherVar;
  public Valid $validObj;

  public function __construct() {
    parent::__construct();

    $this->otherVar = new StringBox();

    $this->validObj = new Valid();

  }

}
