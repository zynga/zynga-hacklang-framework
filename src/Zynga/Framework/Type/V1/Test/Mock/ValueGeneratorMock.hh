<?hh // strict

namespace Zynga\Framework\Type\V1\Test\Mock;

use Zynga\Framework\Type\V1\Test\Mock\BadBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class ValueGeneratorMock extends TestCase {
  private Vector<ValidValue> $validValues = Vector {};
  private Vector<mixed> $invalidValues = Vector {};

  public function hideLogs(): bool {
    return false;
  }
  
  public function getTypeObject(): TypeInterface {
    return new BadBox();
  }

  public function setGenerateValidValues(Vector<ValidValue> $values): void {
    $this->validValues = $values;
  }

  public function generateValidValues(): Vector<ValidValue> {
    return $this->validValues;
  }

  public function setGenerateInvalidValues(Vector<mixed> $values): void {
    $this->invalidValues = $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    return $this->invalidValues;
  }

}
