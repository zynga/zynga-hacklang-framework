<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgModel;
use Zynga\Framework\PgData\V1\PgModel\Reader\GetById;
use Zynga\Framework\PgData\V1\PgModel\SqlGenerator;
use Zynga\Framework\PgData\V1\PgResultSet;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\PgData\V1\PgCachedResultSet;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Environment\Stacktrace\V1\Shortner as StacktraceShortner;

use Zynga\Framework\Type\V1\UInt64Box;

class Reader implements ReaderInterface {

  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  public function getByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $getLocked
  ): ?PgRowInterface {

    try {

      // Snag a reference to the model.
      $pgModel = $this->pgModel();
    
      // --
      // 0) This is a hard performance optimization where we force the fetch from
      //    mc to allow us to skip any work, because we prefer the 'data' segment
      //    cache first.
      // --
      // Tell the cache if we want to get the lock or not
      $cached = $this->fetchSingleRowFromDataCache($model, $id, $getLocked);

      if ($cached instanceof PgRowInterface) {
        $pgModel->stats()->incrementCacheHits();
        return $cached;
      }

      // --
      // Note: the by data caching is handled within the ->get() call so don't duplicate
      // those ops here.
      // --

      // 1) grab a copy of our object to work with.
      $pgModel->stats()->incrementCacheMisses();
      $obj = $pgModel->data()->createRowObjectFromClassName($model);
      $pk = $obj->getPrimaryKeyTyped();
      $pk->set($id);

      // 2) Lock the row for update from the db data.
      $pgModel->cache()->lockRowCache($obj);
      
      // 3) Stand up the where statement to get the response we want.
      $where = new PgWhereClause($pgModel);
      $where->and($obj->getPrimaryKey(), PgWhereOperand::EQUALS, $id);

      // 4) Run the query against the database and dont release any of the lock yet
      $resultSet = $this->fetchResultSetFromDatabase($model, $where, false);

      // 4) We expect a result set of 1 for return.
      if ($resultSet->count() == 1) {
        $row = $resultSet->get(0);
        if ($row instanceof PgRowInterface) {
          // fetchResultSetFromDatabase already writes the row to cache.
          if($getLocked === false) {
            $pgModel->cache()->unlockRowCache($row);
          }
          
          return $row;
        }
      }
      
      // 5) If we got more than 1 result,
      // then release lock for all of them acquired by fetchResultSetFromDatabase
      if($resultSet->count() > 1) {
        foreach ($resultSet->toArray() as $resultObj) {
          if (!$resultObj instanceof PgRowInterface) {
            continue;
          }
          
          $pgModel->cache()->unlockRowCache($resultObj);
        }
        
        return null;
      }

      // 6) row not found by pk. Always unlock in this case
      $pgModel->cache()->unlockRowCache($obj);
      return null;

    } catch (Exception $e) {
      throw $e;
    }
  }

  // As this can return a result set this doesn't let you lock all the items within
  // a result set as the number of edge cases introduced by that logic is not wanted.
  public function get<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    ?PgWhereClauseInterface $where = null,
  ): PgResultSetInterface<PgRowInterface> {

    try {

      // 0) Snag a reference to our pgModel
      $pgModel = $this->pgModel();

      // 1) Make a where if they didn't provide one.
      if ($where == null) {
        $where = new PgWhereClause($pgModel);
      }

      // 2) Attempt to pull a cached result set in from the wild.
      $cachedResults =
        $this->fetchResultSetFromResultSetCache($model, $where);

      if ($cachedResults instanceof PgResultSetInterface) {
        $pgModel->stats()->incrementCacheHits();
        return $cachedResults;
      } else {
        $pgModel->stats()->incrementCacheMisses();
      }

      // 3) Lock the dataset in question for updating.
      $pgModel->cache()->lockResultSetCache($model, $where);

      // 4) Fetch the result set from the database + mc (if needed)
      $resultSet = $this->fetchResultSetFromDatabase($model, $where, true);

      // 5) Save the result set back to cache
      $this->setResultSetToResultSetCache($model, $where, $resultSet);

      // 6) Unlock the dataset
      $pgModel->cache()->unlockResultSetCache($model, $where);

      return $resultSet;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function createCachedResultSet<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $pgWhere,
  ): PgCachedResultSet<TypeInterface> {
    try {

      $pgModel = $this->pgModel();

      $tobj = $pgModel->data()->createRowObjectFromClassName($model);
      $pkTyped = $tobj->getPrimaryKeyTyped();
      $pkType = get_class($pkTyped);

      $cache = new PgCachedResultSet(UInt64Box::class, $pgWhere, $model);

      return $cache;

    } catch (Exception $e) {
      throw $e;
    }
  }

  private function createSql(
    PgRowInterface $row,
    PgWhereClauseInterface $where,
  ): string {
    try {
      $pgModel = $this->pgModel();
      return SqlGenerator::getSelectSql(
        $pgModel->db()->getReadDatabase(),
        $pgModel,
        $row,
        $where,
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  private function fetchResultSetFromDatabase<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
    bool $releaseLockOnSet
  ): PgResultSetInterface<PgRowInterface> {

    try {

      // 0) grab a reference to pgModel
      $pgModel = $this->pgModel();

      // 1) Stand up a result set
      $resultSet = new PgResultSet($model);

      // Snag the pk off the class name.
      $pkKey = $pgModel->data()->getPkFromClassName($model);

      $tobj = $pgModel->data()->createRowObjectFromClassName($model);
      $sql = $this->createSql($tobj, $where);

      // X) Get a database handle.
      $dbh = $pgModel->db()->getReadDatabase();

      // X) Run the query against the database.
      $sth = $dbh->query($sql);

      // X) Increment stats.
      $pgModel->stats()->incrementSqlSelects();

      $wasSuccessful = $sth->wasSuccessful();
      $rowCount = $sth->getNumRows();

      // X) We are done here if the query didn't work or the result set is 0 rows.
      if ($wasSuccessful != true || $rowCount == 0) {
        return $resultSet;
      }

      while ($sth->hasMore() === true && $sth->next() === true) {

        // X) Fetch the row off the db driver.
        $rawRow = $sth->fetchMap();

        // X) create our row object
        $obj = $pgModel->data()->createRowObjectFromClassName($model);

        // X) Take the dataset and lay it into the object
        $pgModel->data()->hydrateDataToRowObject($obj, $rawRow);

        // X) Pull the pk out of the object
        $pk = $obj->getPrimaryKeyTyped();

        // X) Extract what the pk is set to
        $pkValue = $pk->get();

        // X) Fetch cached data to allow for delayed sync back from mc -> db
        // We dont lock on entire data sets
        $cachedRow = $this->fetchSingleRowFromDataCache($model, $pkValue, false);

        if ($cachedRow instanceof PgRowInterface) {
          $resultSet->add($cachedRow);
        } else {
          $pgModel->cache()->lockRowCache($obj);
          $this->setSingleRowToDataCache($obj, $releaseLockOnSet);
          $resultSet->add($obj);
        }

      }

      return $resultSet;

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function fetchResultSetFromResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): ?PgResultSetInterface<PgRowInterface> {

    try {

      $pgModel = $this->pgModel();

      $rsData = $this->createCachedResultSet($model, $where);

      $cache = $pgModel->cache()->getResultSetCache();
      $rsData = $cache->get($rsData);

      if ($rsData instanceof PgCachedResultSet) {
        return $this->thawCachedResultSet($model, $rsData);
      }

      return null;
    } catch (Exception $e) {
      throw $e;
    }

  }

  private function fetchSingleRowFromDataCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $shouldLock
  ): ?PgRowInterface {
    try {

      $pgModel = $this->pgModel();

      $cache = $pgModel->cache()->getDataCache();

      $obj = $pgModel->data()->createRowObjectFromClassName($model);
      $pk = $obj->getPrimaryKeyTyped();
      $pk->set($id);

      $row = $cache->get($obj, $shouldLock);

      if ($row instanceof PgRowInterface) {
        return $row;
      }

      return null;

    } catch (Exception $e) {
      throw $e;
    }
  }

  

  private function setSingleRowToDataCache(PgRowInterface $row, bool $releaseLockOnSet): bool {
    try {

      $pgModel = $this->pgModel();

      $cache = $pgModel->cache()->getDataCache();

      $cache->set($row, $releaseLockOnSet);

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function setResultSetToResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
    PgResultSetInterface<PgRowInterface> $resultSet,
  ): bool {
    try {

      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $this->createCachedResultSet($model, $where);

      foreach ($resultSet->toArray() as $row) {
        $pk = $row->getPrimaryKeyTyped();
        $cachedRs->add($pk);
      }

      // Now inject that bad boy into cache.
      $cache = $pgModel->cache()->getResultSetCache();

      return $cache->set($cachedRs);

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function thawCachedResultSet<TModelClass as PgRowInterface,
  Tv as TypeInterface>(
    classname<TModelClass> $model,
    PgCachedResultSet<Tv> $rsData,
  ): PgResultSetInterface<PgRowInterface> {

    // Create a result set to return.
    $rs = new PgResultSet($model);

    // Thaw the pks into rows.
    foreach ($rsData->toArray() as $pkId) {

      $pk = $pkId->get();

      $obj = $this->getByPk($model, $pk, false);

      if ($obj instanceof PgRowInterface) {
        $rs->add($obj);
      }

    }

    return $rs;
  }

}
