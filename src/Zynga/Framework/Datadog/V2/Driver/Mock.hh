<?hh // strict

namespace Zynga\Framework\Datadog\V2\Driver;

use Zynga\Framework\Datadog\V2\Driver\Base as DriverBase;

/**
 * Mock datadog driver. Intentionally does nothing. Mainly used for testing purposes
 **/
class Mock extends DriverBase {
  /**
   * Inherited from DriverBase.
   **/
  public function timing(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase.
   **/
  public function microTiming(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function gauge(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inerhited from DriverBase
   **/
  public function histogram(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function histogramStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function set(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function event(string $stat, string $value): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function setStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function increment(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function incrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function decrement(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function decrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return true;
  }

  /**
   * Inherited from DriverBase
   **/
  public function serviceCheck(
    string $name,
    int $status,
    ?Map<string, string> $tags = null,
    ?string $hostname = null,
    ?string $message = null,
    ?int $timestamp = null,
  ): bool {
    return true;
  }
}
