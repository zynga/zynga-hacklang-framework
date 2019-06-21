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
   * Add a new item. Will always try to take a look but will only unlock if the api dev asks it
   */
  public function add(PgRowInterface $row, bool $shouldUnlock): bool {

    try {

      $pk = $row->getPrimaryKeyTyped();

      list($isDefaultValue, $isDefaultError) = $pk->isDefaultValue();

      if ($isDefaultValue === true) {

        if ($row->getPrimaryKeyIsFromDatabase() === false) {
          $pk->set($row->getPrimaryKeyNextValue()->get());
        } else {
          throw new Exception(
            'Primary key is default value still. value='.strval($pk->get()),
          );
        }

      }

      $pgModel = $this->pgModel();
      $pgCache = $pgModel->cache();
      
      $locked = $pgCache->lockRowCache($row);
      if ($locked === true) {
        $dataCache = $pgCache->getDataCache();
        $dbh = $pgModel->db()->getWriteDatabase();

        $insertSql = SqlGenerator::getInsertSql($dbh, $pgModel, $row);

        $result = $dbh->query($insertSql);

        if ($result->wasSuccessful() === true) {
          $dataCache->set($row);
          if($shouldUnlock === true) {
            $pgCache->unlockRowCache($row);
          }
          
          return true;
        }
        
        if($shouldUnlock === true) {
          $pgCache->unlockRowCache($row);
        }

      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   * Saves an item.
   * Expects the api dev to have a lock already.
   * Will only unlock if the api dev asks it
   */
  public function save(PgRowInterface $obj, bool $shouldUnlock): bool {
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
      
      if($dataCache->isLocked($obj) === false) {
        throw new Exception('No lock acquired before calling save on obj=' . $obj->export()->asJSON());
      }
      
      $dbh = $pgModel->db()->getWriteDatabase();

      $updateSql = SqlGenerator::getUpdateSql($dbh, $pgModel, $obj);

      $result = $dbh->query($updateSql);

      if ($result->wasSuccessful() === true) {

        $dataCache->set($obj);
        if($shouldUnlock === true) {
          $pgCache->unlockRowCache($obj);
        }
        
        return true;
      }
      
      if($shouldUnlock === true) {
        $pgCache->unlockRowCache($obj);
      }
      
      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }
  
  /**
   * Deletes an item.
   * Expects the api dev to have a lock already.
   * Will only unlock if the api dev asks it
   */
  public function delete(PgRowInterface $obj, bool $shouldUnlock): bool {
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
      
      if($dataCache->isLocked($obj) === false) {
        throw new Exception('No lock acquired before calling save on obj=' . $obj->export()->asJSON());
      }
      
      // Delete from cache first
      if ($dataCache->delete($obj) === true) {
        $dbh = $pgModel->db()->getWriteDatabase();

        $where = new PgWhereClause($pgModel);
        $where->and($obj->getPrimaryKey(), PgWhereOperand::EQUALS, $pk->get());
        $deleteSql = SqlGenerator::getDeleteSql($dbh, $pgModel, $obj, $where);

        $result = $dbh->query($deleteSql);
        if ($result->wasSuccessful() === true) {
          if($shouldUnlock === true) {
            $pgCache->unlockRowCache($obj);
          }
          return true;
        }
      }
      
      return false;

    } catch (Exception $e) {
      throw $e;
    }
  }

}
