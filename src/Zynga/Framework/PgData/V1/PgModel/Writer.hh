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

        error_log('JEO - pk is default');

        if ($row->getPrimaryKeyIsFromDatabase() === false) {
          error_log('JEO - running getNex');
          $pk->set($row->getPrimaryKeyNextValue()->get());
        } else {
          throw new Exception(
            'Primary key is default value still. value='.strval($pk->get()),
          );
        }

      } else {
        error_log('JEO - pk is set?');
      }

      $pgModel = $this->pgModel();

      if ($pgModel->cache()->getDataCache()->lock($row) === true) {

        $dbh = $pgModel->db()->getWriteDatabase();

        $insertSql = SqlGenerator::getInsertSql($dbh, $pgModel, $row);

        $result = $dbh->query($insertSql);

        error_log('insertSql='.$insertSql);

        $pgModel->cache()->getDataCache()->unlock($row);

        if ($result->wasSuccessful() === true) {
          return true;
        }

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

      $id = -1;
      $where = new PgWhereClause($this->pgModel());
      $where->and($obj->getPrimaryKey(), PgWhereOperand::EQUALS, $id);

      $pgModel = $this->pgModel();

      if ($pgModel->cache()->getDataCache()->lock($obj) === true) {

        $dbh = $pgModel->db()->getWriteDatabase();

        $updateSql = SqlGenerator::getUpdateSql($dbh, $pgModel, $obj);

        error_log('updateSql='.$updateSql);

        $pgModel->cache()->getDataCache()->unlock($obj);

      }

    } catch (Exception $e) {
      throw $e;
    }

    return false;

  }

}
