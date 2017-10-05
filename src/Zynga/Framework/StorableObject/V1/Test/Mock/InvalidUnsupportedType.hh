<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Exception\V1\Exception;

class InvalidUnsupportedType extends StorableObject {

  public Exception $example_nonsupported;

  public function __construct() {

    parent::__construct();

    $this->example_nonsupported = new Exception('-not-supported');

  }

}
