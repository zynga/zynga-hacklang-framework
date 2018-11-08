<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

class QuoterTest extends TestCase {

  public function testNormal(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');

    $quoter = $driver->getQuoter();

    $this->assertEquals('0.000000', $quoter->floatValue(0.00));
    $this->assertEquals('123456', $quoter->intValue(123456));
    $this->assertEquals('i am teapot', $quoter->textValue('i am teapot'));

    $vec = Vector {};
    $vec[] = 'you have to catch them all';
    $vec[] = 'as you are the chosen one';
    $this->assertEquals(
      "'you have to catch them all', 'as you are the chosen one'",
      $quoter->textVector($vec),
    );

    DatabaseFactory::clear();

  }

}
