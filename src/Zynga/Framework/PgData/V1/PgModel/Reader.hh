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
  ): ?PgRowInterface {

    try {

      // Snag a reference to the model.
      $pgModel = $this->pgModel();

      // --
      // 0) This is a hard performance optimization where we force the fetch from
      //    mc to allow us to skip any work, because we prefer the 'data' segment
      //    cache first.
      // --
      $cached = $this->fetchSingleRowFromDataCache($model, $id);

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

      // 2) Lock the row for update from the db data.
      $this->lockRowCache($obj);

      // 3) Stand up the where statement to get the response we want.
      $where = new PgWhereClause($pgModel);
      $where->and($obj->getPrimaryKey(), PgWhereOperand::EQUALS, $id);

      // 4) Run the query against the database.
      $resultSet = $this->fetchResultSetFromDatabase($model, $where);

      // 4) We expect a result set of 1 for return.
      if ($resultSet->count() == 1) {
        $row = $resultSet->get(0);
        if ($row instanceof PgRowInterface) {
          if ($this->setSingleRowToDataCache($row) != true) {
            error_log('JEO set to data cache failed, non-critical error?');
          }
          $this->unlockRowCache($obj);
          return $row;
        } else {
          error_log('JEO rows returned contained a non PgRow object?');
        }
      }

      // 5) Row not found by pk.
      $this->unlockRowCache($obj);
      return null;

    } catch (Exception $e) {
      error_log(
        'caughtException e='.$e->getMessage().' eClass='.get_class($e),
      );
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
      $this->lockResultSetCache($model, $where);

      // 4) Fetch the result set from the database + mc (if needed)
      $resultSet = $this->fetchResultSetFromDatabase($model, $where);

      // 5) Save the result set back to cache
      $this->setResultSetToResultSetCache($model, $where, $resultSet);

      // 6) Unlock the dataset
      $this->unlockResultSetCache($model, $where);

      return $resultSet;

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function createCachedResultSet<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $pgWhere,
  ): PgCachedResultSet<TypeInterface> {
    try {

      $pgModel = $this->pgModel();

      $tobj = $pgModel->data()->createRowObjectFromClassName($model);
      $pkTyped = $tobj->getPrimaryKeyTyped();
      $pkType = get_class($pkTyped);

      $backtrace = debug_backtrace();

      if (count($backtrace) > 30) {
        print StacktraceShortner::toString(0, "\n");
        print "\n";
        exit();
      }

      $cache = new PgCachedResultSet(UInt64Box::class, $pgWhere);

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
        $cachedRow = $this->fetchSingleRowFromDataCache($model, $pkValue);

        if ($cachedRow instanceof PgRowInterface) {
          $resultSet->add($cachedRow);
        } else {
          $this->lockRowCache($obj);
          $this->setSingleRowToDataCache($obj);
          $this->unlockRowCache($obj);
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
  ): ?PgRowInterface {
    try {

      $pgModel = $this->pgModel();

      $cache = $pgModel->cache()->getDataCache();

      $obj = $pgModel->data()->createRowObjectFromClassName($model);
      $pk = $obj->getPrimaryKeyTyped();
      $pk->set($id);

      $row = $cache->get($obj);

      if ($row instanceof PgRowInterface) {
        return $row;
      }

      return null;

    } catch (Exception $e) {
      throw $e;
    }
  }

  private function lockRowCache(PgRowInterface $row): bool {

    try {
      $pgModel = $this->pgModel();

      // Now inject that bad boy into cache.
      $cache = $pgModel->cache()->getDataCache();

      return $cache->lock($row);

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function lockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool {
    try {
      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $this->createCachedResultSet($model, $where);

      // Now inject that bad boy into cache.
      $cache = $pgModel->cache()->getResultSetCache();

      return $cache->lock($cachedRs);

    } catch (Exception $e) {
      throw $e;
    }
  }

  private function setSingleRowToDataCache(PgRowInterface $row): bool {
    try {

      $pgModel = $this->pgModel();

      $cache = $pgModel->cache()->getDataCache();

      error_log('JEO set row='.$row->export()->asJSON());
      $cache->set($row);

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

      error_log('JEO thaw id='.strval($pk));
      $obj = $this->getByPk($model, $pk);

      if ($obj instanceof PgRowInterface) {
        error_log('JEO adding id='.strval($pk));
        error_log('JEO obj='.$obj->export()->asJSON());
        $rs->add($obj);
      } else {
        error_log('JEO failed to thaw id='.strval($pk));
      }
    }

    error_log('JEO rsCount='.$rs->count());
    error_log('JEO thawed='.$rs->export()->asJSON());

    return $rs;
  }

  private function unlockRowCache(PgRowInterface $row): bool {

    try {
      $pgModel = $this->pgModel();

      // Now inject that bad boy into cache.
      $cache = $pgModel->cache()->getDataCache();

      return $cache->unlock($row);

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function unlockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool {
    try {
      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $this->createCachedResultSet($model, $where);

      // Now inject that bad boy into cache.
      $cache = $pgModel->cache()->getResultSetCache();

      return $cache->unlock($cachedRs);

    } catch (Exception $e) {
      throw $e;
    }
  }

}
