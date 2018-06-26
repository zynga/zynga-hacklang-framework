<?hh //strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Logging\V1\StaticLogger;

class StaticLoggerTest extends TestCase {
  const string CONTEXT = 'Noop';

  public function test_levels(): void {
    $this->assertTrue(StaticLogger::setHideAllLogs(true), self::CONTEXT);
    $this->assertTrue(StaticLogger::debug('DEBUG - I am', Map {}, true, self::CONTEXT));
    $this->assertTrue(StaticLogger::info('INFO - I am', Map {}, true, self::CONTEXT));
    $this->assertTrue(StaticLogger::warning('WARNING - I am', Map {}, true, self::CONTEXT));
    $this->assertTrue(StaticLogger::error('ERROR - I am', Map {}, true, self::CONTEXT));
    $this->assertTrue(StaticLogger::critical('CRITICAL - I am', Map {}, true, self::CONTEXT));
  }

}
