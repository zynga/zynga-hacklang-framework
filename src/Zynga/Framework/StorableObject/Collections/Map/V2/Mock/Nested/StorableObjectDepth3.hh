<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Type\V1\StringBox;

class StorableObjectDepth3 extends StorableObject {
  public StorableMapCollection<StorableObjectDepth5> $storableMapDepth4;
  public StorableMapCollection<StringBox> $stringMapDepth4;

  public function __construct() {
    parent::__construct();

    $this->storableMapDepth4 = new StorableMap(StorableObjectDepth5::class);
    $this->storableMapDepth4->setIsRequired(true);

    $this->stringMapDepth4 = new StorableMap(StringBox::class);
    $this->stringMapDepth4->setIsRequired(true);
  }
}
