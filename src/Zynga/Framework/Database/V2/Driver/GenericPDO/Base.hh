<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use \PDO;
use \PDOException;
use Zynga\Framework\Database\V2\Driver\Base as BaseDriver;
use Zynga\Framework\Database\V2\Driver\GenericPDO\ConnectionContainer;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Quoter;
use Zynga\Framework\Database\V2\Driver\GenericPDO\ResultSet;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Transaction;
use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\Database\V2\Exceptions\MockQueriesRequired;
use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Exception\V1\Exception;

/**
 * Driver for a GenericPDO connection to a database
 */
class Base extends BaseDriver {

  private ?PDO $_dbh;
  private ?QuoteInterface $_quoter;
  private ?TransactionInterface $_transaction;
  private bool $_connectionState;

  /**
   * See @BaseDriver
   */
  public function __construct(DriverConfigInterface $config) {

    parent::__construct($config);

    $this->_dbh = null;
    $this->_quoter = null;
    $this->_transaction = null;
    $this->_connectionState = false;

  }

  /**
   * See @BaseDriver
   */
  public function getQuoter(): QuoteInterface {
    if ($this->_quoter === null) {
      $this->_quoter = new Quoter($this);
    }

    return $this->_quoter;
  }

  /**
   * See @BaseDriver
   */
  public function getTransaction(): TransactionInterface {
    if ($this->_transaction === null) {
      $this->_transaction = new Transaction($this);
    }

    return $this->_transaction;
  }

  /**
   * See @BaseDriver
   */
  public function setIsConnected(bool $value): bool {
    $this->_connectionState = $value;
    return true;
  }

  /**
   * See @BaseDriver
   */
  public function getIsConnected(): bool {
    return $this->_connectionState;
  }

  /**
   * See @BaseDriver
   */
  public function connectWithString(string $connectionString): bool {

    if ($this->getIsConnected() === true) {
      return true;
    }

    try {

      $username = $this->getConfig()->getUserName();
      $password = $this->getConfig()->getPassword();

      $dbh = new PDO($connectionString, $username, $password);

      // we want our pdo connections to raise exceptions.
      $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      $dbh->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);

      $this->_dbh = $dbh;
      $this->setIsConnected(true);

      return true;
    } catch (PDOException $e) {
      $this->recordError($e->getMessage());
      return false;
    }

  }

  /**
   * See @BaseDriver
   */
  public function connect(): bool {
    $connectionString = $this->getConfig()->getConnectionString();
    return $this->connectWithString($connectionString);
  }

  /**
   * See @BaseDriver
   */
  public function disconnect(): bool {
    $this->_dbh = null;
    return $this->setIsConnected(false);
  }

  /**
   * See @BaseDriver
   */
  public function onDriverConnectionChange(bool $from, bool $to): void {
    // Intentionally empty
  }

  /**
   * See @BaseDriver
   */
  public function query(string $sql): ResultSetInterface {

    try {

      if ($this->getConfig()->isDatabaseReadOnly() === true &&
          $this->isSqlDML($sql) === true) {
        throw new ConnectionIsReadOnly('sql='.$sql);
      }

      if ($this->getIsConnected() !== true) {
        $this->connect();
      }

      // if not connected attempt to connect.
      if ($this->_dbh === null) {
        throw new ConnectionGoneAwayException(
          'NO_CONNECTION host='.$this->getConfig()->getConnectionString(),
        );
      }

      $driver = $this->_dbh;
      $options = array();
      $options[PDO::ATTR_CURSOR] = PDO::CURSOR_SCROLL;
      $query = $driver->prepare($sql, $options);
      $query->execute();
      return new ResultSet($sql, $query);

    } catch (PDOException $e) {
      $this->recordError($e->getMessage());
      throw new QueryFailedException($e->getMessage());
    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   * See @BaseDriver
   */
  public function nativeQuoteString(string $value): string {
    try {

      if ($this->getIsConnected() !== true) {
        $this->connect();
      }

      if ($this->_dbh === null) {
        throw new ConnectionGoneAwayException(
          'NO_CONNECTION host='.$this->getConfig()->getConnectionString(),
        );
      }

      $dbh = $this->_dbh;
      $value = $dbh->quote($value);

      return $value;
    } catch (Exception $e) {
      throw $e;
    }
  }
}
