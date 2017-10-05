<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\FloatBox;

class Valid extends StorableObject {

  public StringBox $example_string;
  public UInt64Box $example_uint64;
  public FloatBox $example_float;

  public function __construct() {

    parent::__construct();

    $this->example_string = new StringBox();
    $this->example_uint64 = new UInt64Box();
    $this->example_float = new FloatBox();

    $this->example_string->setIsRequired(true);
    $this->example_uint64->setIsRequired(false);
    $this->example_float->setIsRequired(true);

  }

}
