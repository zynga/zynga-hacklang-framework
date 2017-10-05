<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\UInt64Box;

class ProtoTestUInt64 extends StorableObject {

  public UInt64Box $example_uint64;

  public function __construct() {

    parent::__construct();

    $this->example_uint64 = new UInt64Box();

  }

}
