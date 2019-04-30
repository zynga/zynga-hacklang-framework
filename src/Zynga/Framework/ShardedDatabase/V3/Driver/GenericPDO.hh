<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Database\V2\Exceptions\MockQueriesRequired;
use Zynga\Framework\ShardedDatabase\V3\Driver\Base;
use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Quoter;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Transaction
;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\ResultSet
;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\ConnectionContainer
;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface
;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface
;
use Zynga\Framework\ShardedDatabase\V3\Info as ShardInfo;

use \PDO;
use \PDOException;

class GenericPDO extends Base {

  private ConnectionContainer $_connections;
  private ?QuoteInterface $_quoter;
  private ?TransactionInterface $_transaction;

  private Map<int, bool> $_connectionState;
  private bool $_hadError;
  private string $_lastError;
  private int $connectedShardId;

  public static bool $FORCE_COVERAGE = false;

  public function __construct(DriverConfigInterface $config) {

    parent::__construct($config);

    $this->_connections = new ConnectionContainer();
    $this->_connectionState = Map {};

    $this->_hadError = false;
    $this->_lastError = '';

    $this->_quoter = null;
    $this->_transaction = null;
    $this->connectedShardId = -1;
  }

  public function getQuoter(): QuoteInterface {
    if ($this->_quoter === null) {
      $this->_quoter = new Quoter($this);
    }

    return $this->_quoter;
  }

  public function getTransaction(): TransactionInterface {
    if ($this->_transaction === null) {
      $this->_transaction = new Transaction($this);
    }

    return $this->_transaction;
  }

  public function setIsConnected(bool $value): bool {
    list($sn, $uid) = $this->getSnUid();
    $shardId = $this->getConfig()->getShardId($sn, $uid);
    $this->_connectionState->set($shardId, $value);
    return true;
  }

  public function getIsConnected(): bool {
    list($sn, $uid) = $this->getSnUid();
    $shardId = $this->getConfig()->getShardId($sn, $uid);
    if ($this->_connectionState->containsKey($shardId) === true &&
        $this->_connectionState[$shardId] === true) {
      return true;
    }

    return false;
  }

  public function connectToShard(
    int $shardIndex,
    string $connectionString,
  ): bool {
    try {
      if ($this->getIsConnected() === true) {
        return true;
      }

      $server = $this->getConfig()->getServerByOffset($shardIndex);
      $username = $server->getUsername();
      $password = $server->getPassword();

      $this->_connections
        ->create($shardIndex, $connectionString, $username, $password);
      $this->setIsConnected(true);
      $this->connectedShardId = $shardIndex;

      return true;
    } catch (Exception $e) {
      $goneAway = new ConnectionGoneAwayException($e->getMessage());
      $goneAway->setPrevious($e);
      throw $goneAway;
    }
  }

  public function connect(): bool {
    if (GenericPDO::$FORCE_COVERAGE === true) {
      return false;
    }
    try {
      list($sn, $uid) = $this->getSnUid();
      $shardIndex = $this->getConfig()->getShardId($sn, $uid);

      // fetch the connection out of the connection pool
      $connectionString = $this->getConfig()->getConnectionString($sn, $uid);
      return $this->connectToShard($shardIndex, $connectionString);
    } catch (MissingUserIdException $e) {
      throw $e;
    } catch (Exception $e) {
      $goneAway = new ConnectionGoneAwayException($e->getMessage());
      $goneAway->setPrevious($e);
      throw $goneAway;
    }
  }

  public function disconnect(): bool {
    $this->_connections->clear();
    $this->_connectionState->clear();

    $this->setIsConnected(false);
    return true;
  }

  public function onDriverConnectionChange(bool $from, bool $to): void {
    // NOOP - for the moment.
  }

  public function hadError(): bool {
    return $this->_hadError;
  }

  public function getLastError(): string {
    return $this->_lastError;
  }

  public function query(string $sql): ResultSetInterface {
    try {
      if ($this->getConfig()->isDatabaseReadOnly() === true &&
          $this->isSqlDML($sql) === true) {
        throw new ConnectionIsReadOnly('sql='.$sql);
      }

      if ($this->getIsConnected() == true) {
        list($sn, $uid) = $this->getSnUid();
        if ($this->connectedShardId != $this->getConfig()->getShardId(
              $sn,
              $uid,
            ) ||
            $this->getConfig()->getCurrentDatabase() != ShardInfo::getDatabaseSchemaForSocialNetworkId(
              $sn,
            )) {
          $this->disconnect();
        }
      }

      if ($this->getIsConnected() !== true) {
        if ($this->connect() !== true) {
          throw new QueryFailedException('Failed to connect');
        }
      }

      $dbh = $this->_connections->get($this->connectedShardId);
      $options = array();
      $options[PDO::ATTR_CURSOR] = PDO::CURSOR_SCROLL;
      $sth = $dbh->prepare($sql, $options);
      $sth->execute();

      return new ResultSet($sql, $sth);
    } catch (PDOException $e) {
      $this->_hadError = true;
      $this->_lastError = $e->getMessage();

      throw new QueryFailedException($e->getMessage());
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function nativeQuoteString(string $value): string {
    try {
      list($sn, $uid) = $this->getSnUid();

      if ($this->getIsConnected() !== true) {
        $this->connect();
      }

      $shardId = $this->getConfig()->getShardId($sn, $uid);
      $dbh = $this->_connections->get($shardId);
      $value = $dbh->quote($value);

      return $value;
    } catch (Exception $e) {
      throw $e;
    }
  }
}
