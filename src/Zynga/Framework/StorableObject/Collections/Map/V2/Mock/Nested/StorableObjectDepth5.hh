<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

class StorableObjectDepth5 extends StorableObject {
  public StorableObjectDepth6 $storableObjectDepth6;

  public function __construct() {
    parent::__construct();

    $this->storableObjectDepth6 = new StorableObjectDepth6();
    $this->storableObjectDepth6->setIsRequired(true);
  }
}
