<?hh // strict

namespace Zynga\Framework\PgData\V1\PgRow;

use Zynga\Framework\Cache\V2\Interfaces\MemcacheDriverInterface;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface as ShardedDriverInterface
;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\PgRow;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;

abstract class PkComesFromMemcache extends PgRow {

  public function getPrimaryKeyIsFromDatabase(): bool {
    return false;
  }

  // --
  // In our use case we pull the id off memcache after reading the pk
  // value from the db.
  // --
  public function getPrimaryKeyNextValue(): TypeInterface {

    try {

      // 0) Stand up a id box.
      $id = new UInt64Box();

      // 1) Bring the cache down locally.
      $cache =
        $this->pgModel()->cache()->getDataCache()->getConfig()->getCache();

      if ($cache instanceof MemcacheDriverInterface) {
        // 2) Pull the write database in
        $writeDatabase = $this->pgModel()->db()->getWriteDatabase();

        // 3) Make a sha256(connection string)|table:pk
        $pkKey = $this->createPkKeyForMC();

        // 4) Attempt to increment via the memcache driver.
        $value = $cache->directIncrement($pkKey);

        // 5) All is well if the value is bigger than 0
        if ($value > 0) {
          $id->set($value);
          return $id;
        }

        // 6) If the value is not bigger than 0 or $shouldLoadFromDatabase = true, attempt to load it from the db.
        $pkKeyLock = $pkKey.':lock';
        $pkLock = $cache->directAdd($pkKeyLock, 0, 0, 30);

        if ($pkLock !== true) {
          throw new Exception('Failed to create pk lock='.$pkKeyLock);
        }

        // 7) Load the value from the db
        $value = $this->loadIdFromDatabase();

        // 8) Save the item to memcache
        if ($cache->directAdd($pkKey, $value) != true) {
          // Unlock the lock if there was some exception here.
          $cache->directDelete($pkKeyLock);

          throw new Exception(
            'Failed to save pkKey='.$pkKey.' to memcache value='.$value,
          );
        }

        $cache->directDelete($pkKeyLock);

        // 9) Pull a new value off mc
        $value = $cache->directIncrement($pkKey);
        $id->set($value);

        return $id;
      }

      throw new Exception(
        'We only support memcache as a driver for PkgComesFromMemcache',
      );
    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   * Delete the primary key value from cache.
   */
  public function deletePrimaryKeyValueFromCache(): bool {
    try {
      $cache =
        $this->pgModel()->cache()->getDataCache()->getConfig()->getCache();

      if ($cache instanceof MemcacheDriverInterface) {
        $pkKey = $this->createPkKeyForMC();
        $pkKeyLock = $pkKey.':lock';
        $pkLock = $cache->directAdd($pkKeyLock, 0, 0, 30);

        if ($pkLock !== true) {
          throw new Exception('Failed to create pk lock='.$pkKeyLock);
        }

        $success = $cache->directDelete($pkKey);
        $cache->directDelete($pkKeyLock);
        return $success;
      }
    } catch (Exception $e) {
      throw $e;
    }

    return false;
  }

  public function getConnectionStringFromWriteDatabase(): string {
    $connectionString = '';

    $writeDatabase = $this->pgModel()->db()->getWriteDatabase();

    if ($writeDatabase instanceof DatabaseDriverInterface) {
      $connectionString = $writeDatabase->getConfig()->getConnectionString();
    }

    if ($connectionString == '') {
      throw new Exception('Connection string needs to be set');
    }

    return $connectionString;
  }

  public function createPkKeyForMC(): string {
    $writeDatabase = $this->pgModel()->db()->getWriteDatabase();
    if($writeDatabase instanceof ShardedDriverInterface) {
      $shardId = $writeDatabase->getConfig()->getShardId($writeDatabase->getShardType());
      $databaseName = $writeDatabase->getConfig()->getDatabaseName();
    } else if($writeDatabase instanceof DatabaseDriverInterface) {
      $shardId = -1;
      $databaseName = $writeDatabase->getConfig()->getCurrentDatabase();
    } else {
      throw new Exception("Unsupported database driver");
    }

    $tableName = $this->getTableName();
    $pkKey = 'pgd:' . hash("sha256", $shardId.':'.$databaseName.":".$tableName) . ':pk';

    return $pkKey;
  }

  public function loadIdFromDatabase(): int {
    try {

      $writeDatabase = $this->pgModel()->db()->getWriteDatabase();

      $tableName = $this->getTableName();

      // --
      // Data has not been loaded yet, let's pull it off the db really quick.
      // --
      $sql = 'SELECT MAX('.$this->getPrimaryKey().') FROM '.$tableName;

      $sth = $writeDatabase->query($sql);

      if ($sth->hasMore() === true && $sth->next() === true) {
        list($id) = $sth->fetchVector();
        return intval($id);
      }

      return 1;

    } catch (Exception $e) {
      throw $e;
    }

  }

}
