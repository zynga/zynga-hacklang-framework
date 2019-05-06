<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableDriverInterface
;
use Zynga\Framework\PgData\V1\Exceptions\PgRowInterfaceRequiredException;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\SqlGenerator;

use \Exception;

abstract class PgModel {

  abstract public function getReadDatabaseName(): string;

  abstract public function getWriteDatabaseName(): string;

  abstract public function getCacheName(): string;

  private function getReadDatabase(): DatabaseDriverInterface {
    return DatabaseFactory::factory(
      DatabaseDriverInterface::class,
      $this->getReadDatabaseName(),
    );
  }

  private function getWriteDatabase(): DatabaseDriverInterface {
    return DatabaseFactory::factory(
      DatabaseDriverInterface::class,
      $this->getWriteDatabaseName(),
    );
  }

  private function getLockableCache(): LockableDriverInterface {
    return LockableCacheFactory::factory(
      LockableDriverInterface::class,
      $this->getCacheName(),
    );
  }

  private function createRowObjectFromClassName<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
  ): PgRowInterface {

    try {
      $obj =
        DynamicClassCreation::createClassByNameGeneric($model, Vector {});

      if ($obj instanceof PgRowInterface) {
        return $obj;
      }

      throw new PgRowInterfaceRequiredException('modelProvided='.$model);

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function createSql(PgRowInterface $row, WhereClause $where): string {
    return
      SqlGenerator::getSql($this->getReadDatabase(), $this, $row, $where);
  }

  private function hydrateDataToRowObject<TModelClass>(
    mixed $obj,
    Map<string, mixed> $rawData,
  ): bool {

    if ($obj instanceof PgRowInterface) {
      return $obj->import()->fromMap($rawData);
    }

    return false;

  }

  public function getById<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $getLocked = false,
  ): ?PgRowInterface {

    try {

      // 1) grab a copy of our object to work with.
      $obj = $this->createRowObjectFromClassName($model);

      // 2) Get a cached version of the object if possible.

      $pk = $obj->getPrimaryKeyTyped();
      $pk->set($id);

      $cache = $this->getLockableCache();
      $cachedObj = $cache->get($obj, $getLocked); // This will apply the lock if asked for.

      if ($cachedObj instanceof PgRowInterface) {
        return $cachedObj;
      }

      // 3) Create sql for the ask to the db.
      $where = new WhereClause();
      $where->and($obj->getPrimaryKey(), WhereOperand::EQUALS, $id);

      $sql = $this->createSql($obj, $where);

      // 4) Get a database handle.
      $dbh = $this->getReadDatabase();

      // 5) Run the query against the database.
      $sth = $dbh->query($sql);

      if ($sth->wasSuccessful() === true && $sth->getNumRows() == 1) {

        $this->hydrateDataToRowObject($obj, $sth->fetchMap());

        $cache->set($obj);

        return $obj;

      }

      return null;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function get<TModelClass>(
    classname<TModelClass> $model,
    ?WhereClause $where = null,
    bool $getLocked = false,
  ): Vector<TModelClass> {
    $data = Vector {};
    return $data;
  }

}

