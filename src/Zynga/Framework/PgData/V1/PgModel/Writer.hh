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

  public function add(PgRowInterface $row): bool {

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

      $cache = $pgModel->cache()->getDataCache();

      if ($cache->lock($row) === true) {

        $dbh = $pgModel->db()->getWriteDatabase();

        $insertSql = SqlGenerator::getInsertSql($dbh, $pgModel, $row);

        $result = $dbh->query($insertSql);

        error_log('insertSql='.$insertSql);

        if ($result->wasSuccessful() === true) {
          $cache->set($row);
          $cache->unlock($row);
          return true;
        }

        $cache->unlock($row);

      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function save(PgRowInterface $obj): bool {
    try {

      $pk = $obj->getPrimaryKeyTyped();

      if ($pk->isDefaultValue() === true) {
        throw new Exception(
          'Primary key is default value still. value='.strval($pk->get()),
        );
      }

      $pgModel = $this->pgModel();

      $cache = $pgModel->cache()->getDataCache();

      if ($cache->lock($obj) === true) {

        $dbh = $pgModel->db()->getWriteDatabase();

        $updateSql = SqlGenerator::getUpdateSql($dbh, $pgModel, $obj);

        error_log('JEO updateSql='.$updateSql);
        $result = $dbh->query($updateSql);

        if ($result->wasSuccessful() === true) {

          $cache->set($obj);

          $cache->unlock($obj);
          return true;
        }

        $cache->unlock($obj);

      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

}
