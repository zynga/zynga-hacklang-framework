<?hh // strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Driver\Base;
use Zynga\Framework\Database\V2\Driver\Vertica\ResultSet as VerticaResultSet;

use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Database\V2\Exceptions\MockQueriesRequired;

use Zynga\Framework\Database\V2\Driver\Vertica\Quoter;
use Zynga\Framework\Database\V2\Driver\Vertica\Transaction;

use Zynga\Framework\Exception\V1\Exception;

class Vertica extends Base {
  private ?TransactionInterface $_transaction;
  private ?QuoteInterface $_quoter;
  private ?resource $_dbh;
  private bool $_connected;

  public function __construct(DriverConfigInterface $config) {
    parent::__construct($config);
    $this->_dbh = null;
    $this->_connected = false;
    $this->_transaction = null;
    $this->_quoter = null;
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

  public function getIsConnected(): bool {
    return $this->_connected;
  }

  public function setIsConnected(bool $state): bool {
    $this->_connected = $state;
    return true;
  }

  public function connectWithString(string $connectionString): bool {

    if ($this->getIsConnected() === true) {
      return true;
    }

    $this->errorCapture()->start();

    $dbh = pg_connect($connectionString);

    $this->errorCapture()->stop();

    if (is_resource($dbh)) {

      $this->setIsConnected(true);

      $this->_dbh = $dbh;

      return true;

    }

    $this->recordError('failed to authenticate');

    return false;

  }

  /**
   * See @BaseDriver
   */
  public function connect(): bool {
    $connectionString = $this->getConfig()->getConnectionString();
    return $this->connectWithString($connectionString);
  }

  public function disconnect(): bool {

    if ($this->getIsConnected() === true && $this->_dbh !== null) {

      $dbh = $this->_dbh;

      $this->errorCapture()->start();

      pg_close($dbh);

      $this->errorCapture()->stop();

    }

    $this->_dbh = null;
    $this->setIsConnected(false);
    return true;

  }

  public function hadError(): bool {

    $lastError = $this->getLastError();

    if ($lastError !== '') {
      $this->recordError($lastError);
    }

    return parent::hadError();

  }

  public function getLastError(): string {
    $errorString = '';
    $error = null;

    $this->errorCapture()->start();

    if ($this->getIsConnected() === true && $this->_dbh !== null) {
      $error = pg_last_error($this->_dbh);
    }

    $this->errorCapture()->stop();

    if (is_string($error)) {
      $errorString = $error;
    }

    return $errorString;

  }

  public function onDriverConnectionChange(bool $from, bool $to): void {
    // NOOP - for the moment.
  }

  public function query(string $sql): ResultSetInterface {

    if ($this->getRequiresMockQueries() === true &&
        !preg_match('/(from|into)\s*dual/i', $sql)) {
      throw new MockQueriesRequired(
        'Your sql does not select from dual sql='.$sql,
      );
    }

    // --
    // JEO: We don't currently have any vertica read only databases.
    // --
    //if ( $this->getConfig()->isDatabaseReadOnly() === true && $this->isSqlDML($sql) === true ) {
    //  throw new ConnectionIsReadOnly('sql=' . $sql);
    //}

    if ($this->getIsConnected() !== true) {
      $this->connect();
    }

    if ($this->_dbh === null) {
      throw new ConnectionGoneAwayException(
        'NO_CONNECTION host='.
        $this->getConfig()->getConnectionString(),
      );
    }

    $dbh = $this->_dbh;

    $this->errorCapture()->start();

    // --
    // Our older driver had this transaction isolation behavior.
    // --
    pg_query($dbh, "set transaction isolation level read uncommitted");

    $rs = pg_query($dbh, $sql);

    $this->errorCapture()->stop();

    if (is_resource($rs)) {
      return new VerticaResultSet($sql, $rs);
    }

    throw new QueryFailedException(
      'QueryException: '.$this->getLastError(),
    );

  }

  public function nativeQuoteString(string $value): string {
    $value = str_replace("'", "\\'", $value);
    return "'$value'";
  }

}
