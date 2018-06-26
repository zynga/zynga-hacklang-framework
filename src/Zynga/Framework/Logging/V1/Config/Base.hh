<?hh // strict

namespace Zynga\Framework\Logging\V1\Config;

use Zynga\Framework\Logging\V1\Interfaces\LoggerConfigInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerAdapterInterface;

abstract class Base implements LoggerConfigInterface {
  private Vector<LoggerAdapterInterface> $_adapters;
  private int $_logLevel = 0;

  public function __construct() {
    $this->_adapters = Vector {};
    $this->init();
  }

  public function addAdapter(LoggerAdapterInterface $adapter): bool {
    $this->_adapters->add($adapter);
    return true;
  }

  public function getAdapters(): Vector<LoggerAdapterInterface> {
    return $this->_adapters;
  }

  public function clearAdapters(): bool {
    $this->_adapters->clear();
    return true;
  }

  public function getLogLevel(): int {
    return $this->_logLevel;
  }

  public function setLogLevel(int $level): bool {

    if ( ($level % 2) !== 0 ) {
      return false;
    }

    $this->_logLevel |= $level;

    return true;

  }

  public function unsetLogLevel(int $level): bool {

    if ( ($level % 2) !== 0 ) {
      return false;
    }

    $this->_logLevel &= ~$level;

    return true;

  }

  public function shouldLog(int $level): bool {
    if ( ($this->_logLevel & $level) == $level ) {
      return true;
    }
    return false;
  }

  abstract public function init(): bool;

  abstract public function getDriver(): string;

}
