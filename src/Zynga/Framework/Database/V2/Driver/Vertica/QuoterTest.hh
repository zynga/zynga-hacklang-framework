<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

class QuoterTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::disableMockDrivers();

    return true;

  }

  public function testFloatValues(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $quoter = $driver->getQuoter();

    $floatValues = Map {};
    $floatValues['0.0'] = 0.0;
    $floatValues['1.0'] = 1.00;
    $floatValues['0.01'] = 0.01;
    $floatValues['0.1789'] = 0.1789;

    foreach ($floatValues as $floatExpected => $floatValue) {
      $this->assertEquals($floatExpected, $quoter->floatValue($floatValue));
    }

    DatabaseFactory::clear();

  }

  public function testIntValues(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $quoter = $driver->getQuoter();

    $intValues = Map {};
    $intValues['0'] = 0;
    $intValues['1'] = 1;
    $intValues['10000'] = 10000;

    $rando = mt_rand(1, mt_getrandmax());
    $intValues["$rando"] = $rando;

    foreach ($intValues as $intExpected => $intValue) {
      $this->assertEquals($intExpected, $quoter->intValue($intValue));
    }

    DatabaseFactory::clear();

  }

  public function testBoolValues(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');
    $quoter = $driver->getQuoter();
    $this->assertEquals('1', $quoter->boolValue(true));
    $this->assertEquals('0', $quoter->boolValue(false));
    DatabaseFactory::clear();
  }

  public function testTextValues(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $quoter = $driver->getQuoter();

    $this->assertEquals(
      "'i am a teapot'",
      $quoter->textValue('i am a teapot'),
    );

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
