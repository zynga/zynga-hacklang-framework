<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\SInt32Box;

class ProtoTestSInt32 extends StorableObject {

  public SInt32Box $example_sint32;

  public function __construct() {

    parent::__construct();

    $this->example_sint32 = new SInt32Box();

  }

}
