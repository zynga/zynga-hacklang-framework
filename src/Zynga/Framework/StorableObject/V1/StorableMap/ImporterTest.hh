<?hh //strict

namespace Zynga\Framework\StorableObject\V1\StorableMap;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\StorableMap;

class ImporterTest extends TestCase {

  public function test_valid(): void {
    $obj = new StorableMap();

    $vec = Vector {};
    $this->assertFalse($obj->import()->fromVector($vec));

    $map = Map {};
    $this->assertFalse($obj->import()->fromMap($map));

    $this->assertFalse($obj->import()->fromJSON(''));
    $this->assertFalse($obj->import()->fromBinary(''));

  }

}
