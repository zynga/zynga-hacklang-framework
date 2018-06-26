<?hh //strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Logging\V1\Factory as LoggingFactory;
use Zynga\Framework\Logging\V1\Interfaces\LoggerInterface;
use Zynga\Framework\Logging\V1\Level;

class FactoryTest extends TestCase {

  public function test_noop(): void {

    $driver = LoggingFactory::factory(LoggerInterface::class, 'Noop');

    $this->assertClassImplements(LoggerInterface::class, $driver);

    $this->assertTrue($driver->error('ERROR: I am a walrus', Map {}));

  }

  public function test_default(): void {

    $driver = LoggingFactory::factory(LoggerInterface::class, 'default');

    $this->assertClassImplements(LoggerInterface::class, $driver);
    $this->assertTrue($driver->error('ERROR: I am a walrus', Map {}));

    // We are going to break the default driver as part of this testing ;)
    // Clear all the currently registered adapters.
    $this->assertTrue($driver->getConfig()->clearAdapters());

    // Fetch the configured log level
    $this->assertEquals(62, $driver->getConfig()->getLogLevel());

    // Test a invalid log level
    $this->assertFalse($driver->getConfig()->setLogLevel(3));

    // Test setting / unsetting levels.
    $this->assertTrue($driver->getConfig()->shouldLog(Level::ERROR));
    $this->assertTrue($driver->getConfig()->unsetLogLevel(Level::ERROR));
    $this->assertFalse($driver->getConfig()->shouldLog(Level::ERROR));

    // Try a invalid log level
    $this->assertFalse($driver->getConfig()->unsetLogLevel(3));

    LoggingFactory::clear();
  }

}
