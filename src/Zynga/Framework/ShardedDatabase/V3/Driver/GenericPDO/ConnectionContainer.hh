<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\InvalidShardException;

use \PDO;
use \PDOException;

use Zynga\Framework\Exception\V1\Exception;

class ConnectionContainer {

  private Map<int, PDO> $_connections = Map {};

  public function isDriverCached(int $shardId): bool {
    if (array_key_exists($shardId, $this->_connections)) {
      return true;
    }
    return false;
  }

  public function create(
    int $shardId,
    string $dsn,
    string $username,
    string $password,
  ): bool {
    try {

      if ($this->isDriverCached($shardId) === true) {
        return true;
      }

      $dbh = new PDO($dsn, $username, $password);
      
      // we want our pdo connections to raise exceptions.
      $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      $dbh->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);

      $this->_connections->set($shardId, $dbh);

      return true;

    } catch (PDOException $e) {
      throw new ConnectionGoneAwayException($e->getMessage());
    }

  }

  public function get(int $shardId): PDO {
    if ($this->isDriverCached($shardId) === true) {
      return $this->_connections[$shardId];
    }
    throw new InvalidShardException('shardId='.$shardId);
  }

  public function clear(): bool {
    $this->_connections->clear();
    return true;
  }

}
