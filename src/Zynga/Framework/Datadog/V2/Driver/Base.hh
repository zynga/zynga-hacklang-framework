<?hh // strict

namespace Zynga\Framework\Datadog\V2\Driver;

use Zynga\Framework\Datadog\V2\Exceptions\MockQueriesRequired;
use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Datadog\V2\Interfaces\DriverInterface;

/**
 * Base implementation of the Datadog driver
 **/
abstract class Base implements DriverInterface {

  /**
   * Configuration for this driver
   **/
  private DriverConfigInterface $_config;

  /**
   * Whether or not mock queries are required for this driver
   **/
  private bool $_requireMockQueries;

  /**
   * Inherited from DriverInterface
   **/
  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    $this->_requireMockQueries = false;
  }

  /**
   * Inherited from DriverInterface
   **/
  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  /**
   * Inherited from DriverInterface
   **/
  public function getRequireMockQueries(): bool {
    return $this->_requireMockQueries;
  }

  /**
   * Inherited from DriverInterface
   **/
  public function setRequireMockQueries(bool $requireMockQueries): bool {
    $this->_requireMockQueries = $requireMockQueries;
    return true;
  }

  /**
   * Inherited from DriverInterface
   **/
  public function shouldSample(
    float $sampleRate,
    bool $forceSample = false,
  ): bool {
    return ($forceSample || (mt_rand() / mt_getrandmax()) <= $sampleRate);
  }
}
