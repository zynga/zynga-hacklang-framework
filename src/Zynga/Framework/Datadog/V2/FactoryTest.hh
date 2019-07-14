<?hh //strict

namespace Zynga\Framework\Datadog\V2;

use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use
  Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface
;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Factory\V1\Exceptions\FailedToLoadDriverConfigException;

use Zynga\Framework\Datadog\V2\ServiceStatus;

class FactoryTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatadogFactory::disableMockDrivers();
    return true;
  }

  public function test_increment_decrement(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $testStat = 'phpunit-test-'.__LINE__;
    $this->assertTrue($dog->increment($testStat));
    $this->assertTrue($dog->decrement($testStat));

    $stats = Vector {};
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $this->assertTrue($dog->incrementStats($stats));
    $this->assertTrue($dog->decrementStats($stats));

    DatadogFactory::clear();
  }

  public function test_timing(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $testStat = 'phpunit-test-'.__LINE__;
    $stamp = microtime(true) - microtime(true);
    $this->assertTrue($dog->microTiming($testStat, $stamp));

    $elapsed = 1.0;
    $testStat = 'phpunit-test-'.__LINE__;
    $this->assertTrue($dog->timing($testStat, $elapsed));

    DatadogFactory::clear();
  }

  public function test_update_times(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');
    $stat = 'playerRatingsTimeOfLastUpdate';
    $timeOfLastUpdate = 0.0;
    $this->assertTrue($dog->set($stat, $timeOfLastUpdate));

    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $stat = 'playerRatingsTimeSinceLastUpdate';
    $now = round(microtime(true) * 1000);
    $timeSinceLastUpdate = $now - $timeOfLastUpdate;
    $this->assertTrue($dog->set($stat, $timeSinceLastUpdate));

    DatadogFactory::clear();
  }

  public function test_guage(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $testStat = 'phpunit-test-'.__LINE__;
    $value = 3.5;
    $this->assertTrue($dog->gauge($testStat, $value));

    DatadogFactory::clear();
  }

  public function test_histogram(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $testStat = 'phpunit-test-'.__LINE__;
    $value = 3.5;
    $this->assertTrue($dog->histogram($testStat, $value));

    $data = Map {};
    $data['phpunit-test-'.__LINE__] = 4.5;
    $data['phpunit-test-'.__LINE__] = 2.1;
    $data['phpunit-test-'.__LINE__] = 1.5;
    $this->assertTrue($dog->histogramStats($data));

    DatadogFactory::clear();
  }

  public function test_set(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $testStat = 'phpunit-test-'.__LINE__;
    $value = 4.5;
    $this->assertTrue($dog->set($testStat, $value));

    $data = Map {};
    $data['phpunit-test-'.__LINE__] = 6.5;
    $data['phpunit-test-'.__LINE__] = 3.1;
    $data['phpunit-test-'.__LINE__] = 89.5;
    $this->assertTrue($dog->setStats($data));

    DatadogFactory::clear();
  }

  public function test_service_check(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');

    $testStat = 'phpunit-test-'.__LINE__;
    $value = 4.5;

    $tags = Map {};
    $tags['tag1'] = 'somevalue';
    $tags['tag2'] = 'someother';

    $hostname = 'www.zynga.com';

    $message = 'I am the walrus';

    $timestamp = time();

    // Walk through all the valid service check values
    $this->assertTrue(
      $dog->serviceCheck(
        $testStat,
        ServiceStatus::OK,
        $tags,
        $hostname,
        $message,
        $timestamp,
      ),
    );
    $this->assertTrue(
      $dog->serviceCheck(
        $testStat,
        ServiceStatus::WARNING,
        $tags,
        $hostname,
        $message,
        $timestamp,
      ),
    );
    $this->assertTrue(
      $dog->serviceCheck(
        $testStat,
        ServiceStatus::CRITICAL,
        $tags,
        $hostname,
        $message,
        $timestamp,
      ),
    );
    $this->assertTrue(
      $dog->serviceCheck(
        $testStat,
        ServiceStatus::UNKNOWN,
        $tags,
        $hostname,
        $message,
        $timestamp,
      ),
    );

    // Test invalid status
    $this->assertFalse(
      $dog->serviceCheck(
        $testStat,
        -1,
        $tags,
        $hostname,
        $message,
        $timestamp,
      ),
    );

    DatadogFactory::clear();
  }

  public function test_sampling(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $stats = Vector {};
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;

    $this->assertTrue($dog->incrementStats($stats, .85));

  }

  public function test_sampling_to_zero(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');

    $stats = Vector {};
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $this->assertFalse($dog->incrementStats($stats, 0.0));
  }

  public function test_tagging(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');

    $tags = Map {};
    $tags['phpunit-tag1'] = sprintf('%d', 1234);
    $tags['phpunit-tag2'] = 'some-value';

    $stats = Vector {};
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $stats[] = 'phpunit-test-'.__LINE__;
    $this->assertTrue($dog->incrementStats($stats, 1.0, $tags));
  }

  <<
  expectedException(
    "Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException",
  )
  >>
  public function test_bad_factory(): void {
    $dog = DatadogFactory::factory(
      DatadogDriverInterface::class,
      'NotARealObject',
    );
  }

}
