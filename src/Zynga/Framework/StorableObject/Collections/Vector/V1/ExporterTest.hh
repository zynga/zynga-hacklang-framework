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

use Zynga\Framework\StorableObject\Collections\Vector\V1\Mock\ValidVectorMock as ValidVectorMock;
use Zynga\Framework\Type\V1\StringBox;

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
  
  public function testValidJSONFromTypeInterface(): void {
    $collectionVector = new ValidVectorMock();
    $stringBox1 = new StringBox();
    $stringBox1->set('123');
    $collectionVector->stringVector->add($stringBox1);
    
    $stringBox2 = new StringBox();
    $stringBox2->set('234');
    $collectionVector->stringVector->add($stringBox2);
    
    $targetJson = '{"stringVector":["123","234"]}';
    $this->assertEquals($targetJson, $collectionVector->export()->asJSON());
  }
}
