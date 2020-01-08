<?hh // strict

namespace Zynga\Framework\Logging\V1\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface as BaseConfigInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerAdapterInterface;

interface LoggerConfigInterface extends BaseConfigInterface {

  /**
   * Adds an adapter to the stack of adapters.
   * @param LoggerAdapterInterface
   * @return bool
   */
  public function addAdapter(LoggerAdapterInterface $adapter): bool;

  /**
   * Fetches the currently configured adapters.
   * @return Vector<LoggerAdapterInterface>
   */
  public function getAdapters(): Vector<LoggerAdapterInterface>;

  /**
   * Removes all adapters from the logging configuration.
   * @return bool
   */
  public function clearAdapters(): bool;

  /**
   * Enables an log level if not already turned on via bitshift.
   * @param int logLevel
   * @return bool
   */
  public function setLogLevel(int $logLevel): bool;

  /**
   * Turns off a specific log level for logging.
   * @param int logLevel
   * @return bool
   */
  public function unsetLogLevel(int $logLevel): bool;

  /**
   * Determins given our configuration if we should perform a logging operation.
   * @param int logLevel
   * @return bool
   */
  public function shouldLog(int $logLevel): bool;

  /**
   * Returns bitmask for the current log level.
   * @return int
   */
  public function getLogLevel(): int;

}
