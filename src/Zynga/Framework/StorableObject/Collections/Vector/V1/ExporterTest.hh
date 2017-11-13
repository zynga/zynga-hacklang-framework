<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\Collections\Exporters\V1\BaseTest as CollectionTest
;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;

class ExporterTest extends CollectionTest {
  protected function getCollection<Tv as StorableObjectInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv> {
    return new Base($classname);
  }

  protected function getCollectionClassName<Tv as StorableObjectInterface>(
  ): classname<StorableCollection<Tv>> {
    return Base::class;
  }

}
