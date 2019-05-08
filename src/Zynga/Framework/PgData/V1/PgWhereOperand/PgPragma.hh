<?hh // strict

namespace Zynga\Framework\PgData\V1\PgWhereOperand;

use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\PgData\V1\PgWhereOperand\PgPragmaType;

class PgPragma {
  private string $_field;
  private PgWhereOperand $_operand;
  private mixed $_value;
  private PgPragmaType $_type;

  public function __construct(
    string $field,
    PgWhereOperand $operand,
    mixed $value,
    PgPragmaType $type,
  ) {
    $this->_field = $field;
    $this->_operand = $operand;
    $this->_value = $value;
    $this->_type = $type;
  }

  public function setField(string $field): bool {
    $this->_field = $field;
    return true;
  }

  public function getField(): string {
    return $this->_field;
  }

  public function setOperand(PgWhereOperand $operand): bool {
    $this->_operand = $operand;
    return true;
  }

  public function getOperand(): PgWhereOperand {
    return $this->_operand;
  }

  public function setValue(mixed $value): bool {
    $this->_value = $value;
    return true;
  }

  public function getValue(): mixed {
    return $this->_value;
  }

  public function setPragmaType(PgPragmaType $type): bool {
    $this->_type = $type;
    return true;
  }

  public function getPragmaType(): PgPragmaType {
    return $this->_type;
  }

}
