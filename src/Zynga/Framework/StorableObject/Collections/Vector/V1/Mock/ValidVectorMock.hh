<?hh // strict
namespace Zynga\Framework\StorableObject\Collections\Vector\V1\Mock;

use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;
use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\Type\V1\StringBox;

class ValidVectorMock extends StorableObjectBase {
  public StorableVector<StringBox> $stringVector;
  
  public function __construct() {
    parent::__construct();
    
    $this->stringVector = new StorableVector(StringBox::class);
    $this->stringVector->setIsRequired(true);
  }
}
