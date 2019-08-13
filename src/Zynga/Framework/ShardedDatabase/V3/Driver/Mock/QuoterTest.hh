<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class QuoterTest extends TestCase {

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    $driver->setShardType(new UInt64Box(1));

    $quoter = $driver->getQuoter();

    $this->assertEquals('0.000000', $quoter->floatValue(0.00));
    $this->assertEquals('123456', $quoter->intValue(123456));
    $this->assertEquals('1', $quoter->boolValue(true));
    $this->assertEquals('0', $quoter->boolValue(false));
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
