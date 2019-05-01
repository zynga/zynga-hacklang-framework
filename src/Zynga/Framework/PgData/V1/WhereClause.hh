<?hh // strict

namespace Zynga\Framework\PgData\V1;

use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\WhereOperand;
use Zynga\Framework\PgData\V1\WhereOperand\FormatStrings;
use Zynga\Framework\PgData\V1\WhereOperand\Pragma;
use Zynga\Framework\PgData\V1\WhereOperand\PragmaType;

use \Exception;

class WhereClause {
  private Vector<Pragma> $_pragma;

  public function __construct() {
    $this->_pragma = Vector {};
  }

  public function and(
    string $field,
    WhereOperand $operand,
    mixed $value,
  ): bool {
    $pragma = new Pragma($field, $operand, $value, PragmaType::AND);
    $this->_pragma->add($pragma);
    return false;
  }

  public function or(
    string $field,
    WhereOperand $operand,
    mixed $value,
  ): bool {
    $pragma = new Pragma($field, $operand, $value, PragmaType::AND);
    $this->_pragma->add($pragma);
    return false;
  }

  public function count(): int {
    return $this->_pragma->count();
  }

  public function buildSql(
    DatabaseDriverInterface $dbh,
    PgRowInterface $row,
  ): string {

    $pragmaCount = $this->_pragma->count();

    error_log('pragmaCount='.$pragmaCount);

    if ($pragmaCount == 0) {
      return '';
    }

    $sql = ' WHERE';

    $offset = 0;
    foreach ($this->_pragma as $pragma) {

      if ($offset != 0 && $pragmaCount > 1) {
        $sql .= $this->convertPragmaTypeToSql($pragma->getPragmaType());
      }

      $sql .= $this->addOperandAndValue($dbh, $row, $pragma);
    }

    error_log('where='.$sql);

    return $sql;

  }

  private function convertPragmaTypeToSql(PragmaType $type): string {
    if (PragmaType::OR) {
      return ' OR ';
    }
    return ' AND ';
  }

  private function addOperandAndValue(
    DatabaseDriverInterface $dbh,
    PgRowInterface $row,
    Pragma $pragma,
  ): string {

    $fieldName = $pragma->getField();

    $field = $row->fields()->getTypedField($fieldName);

    if ($field === null) {
      throw new Exception(
        'Failed to find field='.$fieldName.' on '.get_class($row),
      );
    }

    $value = $this->quoteValue($dbh, $pragma->getValue());

    switch ($pragma->getOperand()) {
      case WhereOperand::EQUALS:
        return sprintf(' %s = %s', $fieldName, $value);
      case WhereOperand::NOT_EQUALS:
        return sprintf(' %s != %s', $fieldName, $value);
      case WhereOperand::LESS_THAN:
        return sprintf(' %s < %s', $fieldName, $value);
      case WhereOperand::GREATER_THAN:
        return sprintf(' %s > %s', $fieldName, $value);
      case WhereOperand::IN:
        return sprintf(' %s IN %s', $fieldName, $value);
      case WhereOperand::NOT_IN:
        return sprintf(' %s NOT IN %s', $fieldName, $value);
    }

    throw new Exception(
      'Unsupported operand type='.
      $pragma->getOperand().
      ' model='.
      get_class($row),
    );

  }

  private function quoteValue(
    DatabaseDriverInterface $dbh,
    mixed $value,
  ): string {

    if (is_string($value)) {
      return $dbh->quote()->textValue($value);
    } else if (is_float($value)) {
      return $dbh->quote()->floatValue($value);
    } else if (is_int($value)) {
      return $dbh->quote()->intValue($value);
    }

    throw new Exception(
      'Unsupported type for quoting value='.gettype($value),
    );

  }

}
