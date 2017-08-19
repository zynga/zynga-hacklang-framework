<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;

class UInt64BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(PHP_INT_MAX);
  }

  public function getTypeObject(): TypeInterface {
    return new UInt64Box();
  }

  public function generateValidValues(): Vector<mixed> {
    $values = parent::generateValidValues();
    $values[] = true;
    $values[] = false;
    return $values;
  }
}
