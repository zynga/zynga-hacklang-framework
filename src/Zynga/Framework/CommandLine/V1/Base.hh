<?hh // strict

namespace Zynga\Framework\CommandLine\V1;

use Zynga\Framework\Performance\V1\Interfaces\PerformanceInterface;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;
use Zynga\Framework\CommandLine\V1\Argument\Container as ArgumentContainer;
use Zynga\Framework\CommandLine\V1\Usage;
use Zynga\Framework\CommandLine\V1\Exceptions\MissingArgumentsException;

use Zynga\Exception;

abstract class Base {

  const string DATE_FORMAT = 'Y-m-d H:i:s';

  private bool $_quietMode;
  private PerformanceInterface $_perf;
  private ArgumentContainer $_args;

  public function __construct() {

    $this->_quietMode = false;
    $this->_perf = new PerformanceTracker();
    $this->_args = new ArgumentContainer();

  }

  public function setupArguments(): bool {
    return true;
  }

  public function convertArgumentsIntoMembers(): bool {
    return true;
  }

  final public function run(): int {
    try {

      $this->setupArguments();

      $this->args()->parse();

      $this->convertArgumentsIntoMembers();

      return $this->handleRun();

    } catch (MissingArgumentsException $e) {
      return $this->runHandleCaughtMissingArgumentsException($e);
    } catch (Exception $e) {
      return $this->runHandleCaughtException($e);
    }
  }

  abstract public function handleRun(): int;

  public function runHandleCaughtMissingArgumentsException(
    MissingArgumentsException $e,
  ): int {

    $this->error($e->getMessage());

    // dump out the usage
    $usageObj = new Usage();
    $usageObj->createUsage($this->args(), inst_meth($this, 'error'));

    return 255;
  }

  public function runHandleCaughtException(Exception $e): int {
    $this->error(
      'Caught exception='.get_class($e).' message='.$e->getMessage(),
    );
    $this->error('STACKTRACE: ');
    $this->error($e->getTraceAsString());
    $this->error('FAIL');
    return 255;
  }

  public function args(): ArgumentContainer {
    return $this->_args;
  }

  public function getQuietMode(): bool {
    return $this->_quietMode;
  }

  public function enableQuietMode(): bool {
    $this->_quietMode = true;
    return true;
  }

  public function getTime(): int {
    $time = time();
    return $time;
  }

  public function error(string $message): void {
    $this->message('[ERROR] - '.$message);
  }

  public function info(string $message): void {
    $this->message('[INFO] - '.$message);
  }

  public function message(string $message): bool {

    if ($this->_quietMode === true) {
      return true;
    }

    $time = $this->getTime();

    echo date(self::DATE_FORMAT, $time).' '.$message."\n";

    return true;

  }

  public function perf(): PerformanceInterface {
    return $this->_perf;
  }

}
