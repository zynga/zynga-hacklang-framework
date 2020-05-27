<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\PgModel\SqlGenerator;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;

class Writer implements WriterInterface {
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  /**
   * Add a new item.
   */
  public function add(PgRowInterface $row): bool {

    $wasSuccessful = $this->addToDbOnly($row);
    if ($wasSuccessful) {
      $pgModel = $this->pgModel();
      $pgCache = $pgModel->cache();
      $dataCache = $pgCache->getDataCache();
      $dataCache->set($row);

    }
    return $wasSuccessful;
  }

  public function addToDbOnly(PgRowInterface $row): bool {
    $pk = $row->getPrimaryKeyTyped();

    list($isDefaultValue, $isDefaultError) = $pk->isDefaultValue();

    if ($isDefaultValue === true) {
      $retryOnException = true;
      if ($row->getPrimaryKeyIsFromDatabase() === false) {
        $pk->set($row->getPrimaryKeyNextValue(false)->get());
      } else {
        throw new Exception(
          'Primary key is default value still. value='.strval($pk->get()),
        );
      }
    } else {
      $retryOnException = false;
    }

    try {
      return $this->addNewRowToDB($row);
    } catch (Exception $e) {
      if($retryOnException == true) {
        // Fetch the pk value from DB and update memcache;
        $pk->set($row->getPrimaryKeyNextValue(true)->get());
        return $this->addNewRowToDB($row);
      }
    }

    return false;
  }

  private function addNewRowToDB(PgRowInterface $row): bool {
    $pgModel = $this->pgModel();
    $dbh = $pgModel->db()->getWriteDatabase();

    $insertSql = SqlGenerator::getInsertSql($dbh, $pgModel, $row);

    $result = $dbh->query($insertSql);

    return $result->wasSuccessful();
  }

  /**
   * Saves an item.
   * Expects the api dev to have a lock already.
   * Will only unlock if the api dev asks it
   */
  public function save(PgRowInterface $obj, bool $shouldUnlock): bool {
    $pgCache = null;
    try {

      $pk = $obj->getPrimaryKeyTyped();

      if ($pk->isDefaultValue() === true) {
        throw new Exception(
          'Primary key is default value still. value='.strval($pk->get()),
        );
      }

      $pgModel = $this->pgModel();

      $pgCache = $pgModel->cache();
      $dataCache = $pgCache->getDataCache();

      if ($dataCache->isLockedByMyThread($obj) === false) {
        throw new Exception(
          'No lock acquired before calling save on obj='.
          $obj->export()->asJSON(),
        );
      }

      $dbh = $pgModel->db()->getWriteDatabase();

      $updateSql = SqlGenerator::getUpdateSql($dbh, $pgModel, $obj);

      $result = $dbh->query($updateSql);

      if ($result->wasSuccessful() === true) {

        $dataCache->set($obj);

        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    } finally {
      if ($shouldUnlock === true && $pgCache !== null) {
        $pgCache->unlockRowCache($obj);
      }
    }
  }

  /**
   * Deletes an item.
   * Expects the api dev to have a lock already.
   * Will only unlock if the api dev asks it
   */
  public function delete(PgRowInterface $obj, bool $shouldUnlock): bool {
    $pgCache = null;
    try {

      $pk = $obj->getPrimaryKeyTyped();

      if ($pk->isDefaultValue() === true) {
        throw new Exception(
          'Primary key is default value still. value='.strval($pk->get()),
        );
      }

      $pgModel = $this->pgModel();
      $pgCache = $pgModel->cache();
      $dataCache = $pgCache->getDataCache();

      if ($dataCache->isLockedByMyThread($obj) === false) {
        throw new Exception(
          'No lock acquired before calling save on obj='.
          $obj->export()->asJSON(),
        );
      }

      // Tombstone from cache first
      if ($obj->tombstoneRow() === true && $dataCache->set($obj) === true) {
        $dbh = $pgModel->db()->getWriteDatabase();

        $where = new PgWhereClause($pgModel);
        $where->and(
          $obj->getPrimaryKey(),
          PgWhereOperand::EQUALS,
          $pk->get(),
        );
        $deleteSql = SqlGenerator::getDeleteSql($dbh, $pgModel, $obj, $where);

        $result = $dbh->query($deleteSql);
        if ($result->wasSuccessful() === true) {
          return true;
        }
      }
      return false;

    } catch (Exception $e) {
      throw $e;
    } finally {
      if ($shouldUnlock === true && $pgCache !== null) {
        $pgCache->unlockRowCache($obj);
      }
    }
  }

}
