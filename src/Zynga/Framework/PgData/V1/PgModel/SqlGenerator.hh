<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use Zynga\Framework\PgData\V1\Exceptions\NoFieldsOnObjectException;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class SqlGenerator {

  public static function getSelectSql(
    QueryableInterface $dbh,
    PgModelInterface $model,
    PgRowInterface $obj,
    PgWhereClauseInterface $where,
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

      return $sql;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function getInsertSql(
    QueryableInterface $dbh,
    PgModelInterface $model,
    PgRowInterface $obj,
  ): string {

    try {

      // 0) Snag all the fields off the object.
      $fieldMap = $obj->fields()->getFieldsAndTypesForObject();

      // 1) Verify that this object has fields
      if ($fieldMap->count() == 0) {
        // this storable is invalid as it has no fields.
        throw new NoFieldsOnObjectException('obj='.get_class($obj));
      }

      $fields = Vector {};
      $quotedValues = Vector {};

      foreach ($fieldMap as $fieldName => $fieldType) {

        // push the field name onto the stack.
        $fields->add($fieldName);

        // Fetch the value for the name
        $fieldObj = $obj->fields()->getTypedField($fieldName);
        $fieldValue = $fieldObj->get();

        // Quote the value and push it onto the insertion stack
        $quotedValues->add($model->db()->quoteValue($dbh, $fieldValue));

      }

      // 3) snag the table name off the obj
      $tableName = $obj->getTableName();

      $sql =
        'INSERT INTO '.
        $tableName.
        ' ( '.
        implode(',', $fields).
        ') VALUES ( '.
        implode(',', $quotedValues).
        ')';

      return $sql;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function getUpdateSql(
    QueryableInterface $dbh,
    PgModelInterface $model,
    PgRowInterface $obj,
  ): string {

    try {
      $pk = $obj->getPrimaryKeyTyped();
      $pkName = $obj->getPrimaryKey();

      $quotedValues = Vector {};

      $fieldMap = $obj->fields()->getFieldsAndTypesForObject();
      if ($fieldMap->count() == 0) {
        throw new NoFieldsOnObjectException('obj='.get_class($obj));
      }

      foreach ($fieldMap as $fieldName => $fieldType) {

        // skip the pk.
        if ($fieldName == $pkName) {
          continue;
        }

        $fieldObj = $obj->fields()->getTypedField($fieldName);
        $fieldValue = $fieldObj->get();

        $quotedValue =
          $fieldName.' = '.$model->db()->quoteValue($dbh, $fieldValue);

        $quotedValues->add($quotedValue);

      }

      $id = $pk->get();
      $where = new PgWhereClause($model);
      $where->and($obj->getPrimaryKey(), PgWhereOperand::EQUALS, $id);
      $whereClause = $where->buildSql($dbh, $obj);

      $tableName = $obj->getTableName();

      $sql =
        'UPDATE '.$tableName.' SET '.implode(',', $quotedValues).$whereClause;

      return $sql;

    } catch (Exception $e) {
      throw $e;
    }
  }

}
