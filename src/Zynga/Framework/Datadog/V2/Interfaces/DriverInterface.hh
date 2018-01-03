<?hh // strict

namespace Zynga\Framework\Datadog\V2\Interfaces;

use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as BaseDriverInterface
;

interface DriverInterface extends BaseDriverInterface {
  /**
   * Returns whether or not mock queries are required for this driver
   *
   * @return True if mock queries are required for this driver, otherwise false
   **/
  public function getRequireMockQueries(): bool;

  /**
   * Sets whether or not mock queries are required for this driver
   *
   * @param $setRequireMockQueries True if mock queries are required for this
   *        driver, otherwise false.
   **/
  public function setRequireMockQueries(bool $setRequireMockQueries): bool;

  /**
   * Returns the configuration for this driver
   *
   * @return The configuration for this driver
   **/
  public function getConfig(): DriverConfigInterface;

  /**
   * Returns whether or not the next stat should be sampled
   *
   * @param $sampleRate The rate at which to sample
   * @param $forceSample True if the stat should be forced to be sampled, otherwise false.
   * @return True if the next stat should be sampled, otherwise false.
   **/
  public function shouldSample(float $sampleRate, bool $forceSample): bool;

  /**
   * Log timing information
   *
   * @param string $stat The metric to in log timing info for.
   * @param float $time The elapsed time in seconds to log
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param ?Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function timing(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Recording the elapsed time from microtime(true) - microtime(true) = elapsed (f)
   *
   * @param string $stat The metric name
   * @param float $time The elapsed time in microseconds to log
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param array|string $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function microTiming(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Gauge
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param array|string $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function gauge(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Histogram
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function histogram(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Histogram
   *
   * @param Map<string,float> $stats The metrics you would like to update
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function histogramStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Set
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function set(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Logs a single event
   *
   * @param $stat The metric name
   * @param $value The value to be associated with the metric
   * @return True if successful, otherwise false
   **/
  public function event(string $stat, string $value): bool;

  /**
   * setStats
   *
   * @param Map<string, float> $stats The metrics you would like to update
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function setStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Increments one stat counter
   *
   * @param string $stat The metric to increment.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function increment(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Increments multiple stat counters
   *
   * @param Vector<string> $stats The metrics to increment.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function incrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Decrements one stat counter
   *
   * @param string $stat The metric to decrement.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function decrement(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Decrements multiple stat counters
   *
   * @param Vector<string> $stats The metrics to decrement.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function decrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool;

  /**
   * Send a custom service check status.
   * @param string $name service check name
   * @param int $status service check status code (see static::OK, static::WARNING,...)
   * @param ?Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @param ?string $hostname hostname to associate with this service check status
   * @param ?string $message message to associate with this service check status
   * @param ?int $timestamp timestamp for the service check status (defaults to now)
   * @return bool
   **/
  public function serviceCheck(
    string $name,
    int $status,
    ?Map<string, string> $tags = null,
    ?string $hostname = null,
    ?string $message = null,
    ?int $timestamp = null,
  ): bool;
}
