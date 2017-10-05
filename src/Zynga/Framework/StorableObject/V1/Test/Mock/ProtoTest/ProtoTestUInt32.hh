<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\UInt32Box;

class ProtoTestUInt32 extends StorableObject {

  public UInt32Box $example_uint32;

  public function __construct() {

    parent::__construct();

    $this->example_uint32 = new UInt32Box();

  }

}
