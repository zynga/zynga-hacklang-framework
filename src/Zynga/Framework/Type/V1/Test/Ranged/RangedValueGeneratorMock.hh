<?hh // strict

namespace Zynga\Framework\Type\V1\Test\Ranged;

use Zynga\Framework\Type\V1\Test\Mock\BadBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class RangedValueGeneratorMock extends TestCase {
  private Vector<ValidValue> $validValues = Vector {};
  private Vector<mixed> $invalidValues = Vector {};

  public function hideLogs(): bool {
    return false;
  }

  public function setUp(): void {
    parent::setUp();
    $this->setMin(PHP_INT_MIN);
    $this->setMax(PHP_INT_MAX);
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
