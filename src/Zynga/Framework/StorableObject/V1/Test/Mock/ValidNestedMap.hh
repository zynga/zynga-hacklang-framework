<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\StorableObject\V1\StorableMap;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;

class ValidNestedMap extends StorableObjectBase {
  public StorableMap<StringBox> $stringMap;
  public StorableMap<Valid> $validMap;

  public function __construct() {

    parent::__construct();

    $this->stringMap = new StorableMap();
    $this->validMap = new StorableMap();

  }

}
