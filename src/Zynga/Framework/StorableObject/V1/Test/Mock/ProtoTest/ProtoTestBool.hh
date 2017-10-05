<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\BoolBox;

class ProtoTestBool extends StorableObject {

  public BoolBox $example_bool;

  public function __construct() {

    parent::__construct();

    $this->example_bool = new BoolBox();

  }

}
