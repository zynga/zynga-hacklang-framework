<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection as StorableVectorCollection
;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector
;
use Zynga\Framework\Type\V1\BoolBox;

class StorableObjectDepth2 extends StorableObject {
  public StorableVectorCollection<BoolBox> $boolVectorDepth3;
  public StorableObjectDepth3 $storableObjectDepth3;

  public function __construct() {
    parent::__construct();

    $this->boolVectorDepth3 = new StorableVector(BoolBox::class);
    $this->boolVectorDepth3->setIsRequired(true);

    $this->storableObjectDepth3 = new StorableObjectDepth3();
    $this->storableObjectDepth3->setIsRequired(true);
  }
}
