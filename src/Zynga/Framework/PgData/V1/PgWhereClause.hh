<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use Zynga\Framework\PgData\V1\Exceptions\FailedToFindFieldOnObjectException;
use Zynga\Framework\PgData\V1\Exceptions\UnsupportedOperandException;
use Zynga\Framework\PgData\V1\Exceptions\UnsupportedValueTypeException;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\PgData\V1\PgWhereOperand\PgPragma;
use Zynga\Framework\PgData\V1\PgWhereOperand\PgPragmaType;

use \Exception;

class PgWhereClause implements PgWhereClauseInterface {
  private Vector<PgPragma> $_pragmas;
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pragmas = Vector {};
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  public function and(
    string $field,
    PgWhereOperand $operand,
    mixed $value,
  ): bool {
    $pragma = new PgPragma($field, $operand, $value, PgPragmaType::AND);
    $this->_pragmas->add($pragma);
    return false;
  }

  public function or(
    string $field,
    PgWhereOperand $operand,
    mixed $value,
  ): bool {
    $pragma = new PgPragma($field, $operand, $value, PgPragmaType::AND);
    $this->_pragmas->add($pragma);
    return false;
  }

  public function count(): int {
    return $this->_pragmas->count();
  }

  public function buildSql(
    QueryableInterface $dbh,
    PgRowInterface $row,
  ): string {

    $pragmaCount = $this->count();

    error_log('pragmaCount='.$pragmaCount);

    if ($pragmaCount == 0) {
      return '';
    }

    $sql = ' WHERE';

    $offset = 0;
    foreach ($this->_pragmas as $pragma) {

      if ($offset != 0 && $pragmaCount > 1) {
        $sql .= $this->convertPragmaTypeToSql($pragma->getPragmaType());
      }

      $sql .= $this->addOperandAndValue($dbh, $row, $pragma);

      $offset++;

    }

    error_log('JEO where='.$sql);

    return $sql;

  }

  private function convertPragmaTypeToSql(PgPragmaType $type): string {
    if (PgPragmaType::OR) {
      return ' OR ';
    }
    return ' AND ';
  }

  private function addOperandAndValue(
    QueryableInterface $dbh,
    PgRowInterface $row,
    PgPragma $pragma,
  ): string {

    $fieldName = $pragma->getField();

    $field = $row->fields()->getTypedField($fieldName);

    if ($field === null) {
      throw new FailedToFindFieldOnObjectException(
        'Failed to find field='.$fieldName.' on '.get_class($row),
      );
    }

    $value = $this->pgModel()->db()->quoteValue($dbh, $pragma->getValue());

    switch ($pragma->getOperand()) {
      case PgWhereOperand::EQUALS:
        return sprintf(' %s = %s', $fieldName, $value);
      case PgWhereOperand::NOT_EQUALS:
        return sprintf(' %s != %s', $fieldName, $value);
      case PgWhereOperand::LESS_THAN:
        return sprintf(' %s < %s', $fieldName, $value);
      case PgWhereOperand::GREATER_THAN:
        return sprintf(' %s > %s', $fieldName, $value);
      case PgWhereOperand::IN:
        return sprintf(' %s IN %s', $fieldName, $value);
      case PgWhereOperand::NOT_IN:
        return sprintf(' %s NOT IN %s', $fieldName, $value);
    }

    throw new UnsupportedOperandException(
      'type='.$pragma->getOperand().' model='.get_class($row),
    );

  }

  public function createWhereChecksum(): string {

    $params = '';

    foreach ($this->_pragmas as $pragma) {

      $params .=
        $pragma->getField().
        '|'.
        $pragma->getOperand().
        '|'.
        strval($pragma->getValue()).
        "\n";

    }

    $checksum = md5($params);
    error_log('createWhereChecksum params='.$params.' checksum='.$checksum);

    return $checksum;
  }

}
