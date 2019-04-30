<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class QuoterTest extends TestCase {

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    $driver->setSnUid(SnidBoxFactory::facebook(), new UidBox(1));

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
