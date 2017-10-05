<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\Int64Box;

class ProtoTestInt64 extends StorableObject {

  public Int64Box $example_int64;

  public function __construct() {

    parent::__construct();

    $this->example_int64 = new Int64Box();

  }

}
