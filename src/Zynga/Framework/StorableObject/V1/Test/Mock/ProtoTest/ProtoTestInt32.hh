<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\Int32Box;

class ProtoTestInt32 extends StorableObject {

  public Int32Box $example_int32;

  public function __construct() {

    parent::__construct();

    $this->example_int32 = new Int32Box();

  }

}
