<?hh //strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Logging\V1\Factory as LoggingFactory;
use Zynga\Framework\Logging\V1\Interfaces\LoggerInterface;
use Zynga\Framework\Logging\V1\Level;
use Zynga\Framework\Logging\V1\Driver\Base as DriverBase;

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

  public function test_backtraceEmpty(): void {
    $driver = LoggingFactory::factory(LoggerInterface::class, 'default');

    if ($driver instanceof DriverBase) {
      $this->assertEquals('', $driver->formatBacktrace(null));
    }

  }

  public function test_loggingFilters(): void {

    $driver = LoggingFactory::factory(LoggerInterface::class, 'NoLevelsSet');

    $this->assertFalse(
      $driver->error(
        'this shoul not get logged - sample rate lower than pct',
        Map {},
        false,
        0.0,
      ),
    );

    $this->assertFalse(
      $driver->error(
        'this shoul not get logged - no level to make it log',
        Map {},
      ),
    );

  }

}
