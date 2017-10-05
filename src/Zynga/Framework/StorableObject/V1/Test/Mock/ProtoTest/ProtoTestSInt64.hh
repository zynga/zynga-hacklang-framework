<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\SInt64Box;

class ProtoTestSInt64 extends StorableObject {

  public SInt64Box $example_sint64;

  public function __construct() {

    parent::__construct();

    $this->example_sint64 = new SInt64Box();

  }

}
