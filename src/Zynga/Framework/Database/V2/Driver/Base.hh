<?hh // strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Database\V2\Exceptions\MockQueriesRequired;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface;
use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Noop as ErrorCaptureNoop;
use Zynga\Framework\Factory\V2\Driver\Base as FactoryDriverBase;

abstract class Base extends FactoryDriverBase implements DriverInterface {
  private bool $_requireMockQueries;

  private DriverConfigInterface $_config;
  private ?QuoteInterface $_quoter = null;
  private ?TransactionInterface $_transaction = null;
  private ErrorCaptureInterface $_errorCapture;
  private bool $_hadError;
  private string $_lastError;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    $this->_errorCapture = new ErrorCaptureNoop();
    $this->_requireMockQueries = false;
    $this->_hadError = false;
    $this->_lastError = '';
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function getRequiresMockQueries(): bool {
    return $this->_requireMockQueries;
  }

  public function enableRequireMockQueries(): bool {
    $this->_requireMockQueries = true;
    return true;
  }

  public function disableRequireMockQueries(): bool {
    $this->_requireMockQueries = false;
    return true;
  }

  public function quote(): QuoteInterface {
    return $this->getQuoter();
  }

  public function transaction(): TransactionInterface {
    return $this->getTransaction();
  }

  abstract public function setIsConnected(bool $state): bool;

  abstract public function getIsConnected(): bool;

  public function isConnected(): bool {
    return $this->getIsConnected();
  }

  public function isSqlDML(string $sql): bool {
    if (preg_match('/^(TRUNCATE|REPLACE|INSERT|UPDATE|DELETE)/i', $sql)) {
      return true;
    }
    return false;
  }

  public function errorCapture(): ErrorCaptureInterface {
    return $this->_errorCapture;
  }


  public function hadError(): bool {
    return $this->_hadError;
  }

  public function recordError(string $errorMessage): bool {
    $this->_hadError = true;
    $this->_lastError = $errorMessage;
    return true;
  }

  public function getLastError(): string {
    return $this->_lastError;
  }

}
