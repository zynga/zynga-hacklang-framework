<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\Collections\Importers\V1\BoxTest as CollectionTest
;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class BoxImporterTest extends CollectionTest {
  protected function getCollection<Tv as TypeInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv> {
    return new Base($classname);
  }

}
