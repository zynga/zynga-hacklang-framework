<?hh // strict

namespace Zynga\Framework\PgData\V1;

use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\PgData\V1\Exceptions\NoFieldsOnObjectException;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\WhereClause;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class SqlGenerator {

  public static function getSelectSql(
    DatabaseDriverInterface $dbh,
    PgModelInterface $model,
    PgRowInterface $obj,
    WhereClause $where,
  ): string {

    try {
      // 0) Snag all the fields off the object.
      $fieldMap = $obj->fields()->getFieldsAndTypesForObject();

      // 1) Verify that this object has fields
      if ($fieldMap->count() == 0) {
        // this storable is invalid as it has no fields.
        throw new NoFieldsOnObjectException('obj='.get_class($obj));
      }

      // 2) Build a list of fields to include on the select
      $selectFields = Vector {};
      foreach ($fieldMap as $fieldName => $fieldType) {
        $selectFields->add($fieldName);
      }

      // 3) snag the table name off the obj
      $tableName = $obj->getTableName();

      // 4) Build our where stack.
      $whereSql = $where->buildSql($dbh, $obj);

      $sql =
        'SELECT '.implode(', ', $selectFields).' FROM '.$tableName.$whereSql;

      error_log('selectSql='.$sql);
      return $sql;

    } catch (Exception $e) {
      throw $e;
    }

  }
}
