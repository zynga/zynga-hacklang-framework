<?hh // strict

namespace Zynga\Framework\PgData\V1\WhereOperand;

use Zynga\Framework\PgData\V1\WhereOperand;
use Zynga\Framework\PgData\V1\WhereOperand\PragmaType;

class Pragma {
  private string $_field;
  private WhereOperand $_operand;
  private mixed $_value;
  private PragmaType $_type;

  public function __construct(
    string $field,
    WhereOperand $operand,
    mixed $value,
    PragmaType $type,
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

  public function setOperand(WhereOperand $operand): bool {
    $this->_operand = $operand;
    return true;
  }

  public function getOperand(): WhereOperand {
    return $this->_operand;
  }

  public function setValue(mixed $value): bool {
    $this->_value = $value;
    return true;
  }

  public function getValue(): mixed {
    return $this->_value;
  }

  public function setPragmaType(PragmaType $type): bool {
    $this->_type = $type;
    return true;
  }

  public function getPragmaType(): PragmaType {
    return $this->_type;
  }

}
