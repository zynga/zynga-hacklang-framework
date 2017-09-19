<?hh // strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Database\V2\Driver\Base;
use Zynga\Framework\Database\V2\Driver\Mock\ResultSet;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Database\V2\Mock\ResultSets;

class Mock extends Base {
  private int $_queryCounter;
  private bool $_isConnected;
  private bool $_hadError;
  private int $_resultOffset;
  private ?QuoteInterface $_quoter;
  private ?TransactionInterface $_transaction;
  private ResultSets $_resultSets;

  public function __construct(DriverConfigInterface $config) {

    parent::__construct($config);

    $this->_isConnected = false;
    $this->_queryCounter = 0;
    $this->_hadError = false;
    $this->_resultOffset = 0;
    $this->_quoter = null;
    $this->_transaction = null;
    $this->_resultSets = new ResultSets();

  }

  public function getTransaction(): TransactionInterface {
    if ( $this->_transaction === null ) {
      $this->_transaction = new Transaction($this);
    }
    return $this->_transaction;
  }

  public function getQuoter(): QuoteInterface {
    if ( $this->_quoter === null ) {
      $this->_quoter = new Quoter($this);
    }
    return $this->_quoter;
  }

  public function connect(): bool {
    $this->_isConnected = true;
    return true;
  }

  public function disconnect(): bool {
    $this->_isConnected = false;
    return true;
  }

  public function setIsConnected(bool $state): bool {
    $this->_isConnected = $state;
    return true;
  }

  public function getIsConnected(): bool {
    return $this->_isConnected;
  }

  public function getLastError(): string {
    return 'NOOP';
  }

  public function hadError(): bool {
    return $this->_hadError;
  }

  public function nativeQuoteString(string $value): string {
    return $value;
  }

  public function query(string $sql): ResultSetInterface {

    try {

      $this->_queryCounter++;

      $rsOffset = $this->_resultOffset;

      $rsData = $this->getResultSet(
        '['.$this->_queryCounter.'] query="'.$sql.'"',
        $rsOffset,
      );

      if ($rsData->get(0) === false) {
        throw new QueryFailedException('The query failed! sql='.$sql);
      }

      $rs = new ResultSet();

      $rs->setSql($sql);
      $rs->setResultData($rsData);

      $this->_resultOffset++;

      return $rs;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function resetResultsSets(): bool {
    $this->_resultSets->clear();
    $this->_isConnected = false;
    $this->_queryCounter = 0;
    $this->_hadError = false;
    $this->_resultOffset = 0;
    return true;
  }

  public function loadResultsForTest(
    string $class,
    string $functionName,
  ): bool {
    try {
      $this->resetResultsSets();
      return $this->_resultSets->loadResultsForTest($class, $functionName);
    } catch ( Exception $e ) {
      throw $e;
    }
  }

  public function addEmptyResultSet(): bool {
    return $this->_resultSets->addEmptyResultSet();
  }

  public function addFailingResultSet(): bool {
    return $this->_resultSets->addFailingResultSet();
  }

  public function addResultSet(Vector<mixed> $resultSet): bool {
    return $this->_resultSets->addResultSet($resultSet);
  }

  public function getResultSet(string $context, int $rsOffset): Vector<mixed> {
    try {
      return $this->_resultSets->getResultSet($context, $rsOffset);
    } catch (Exception $e) {
      throw $e;
    }
  }

}
