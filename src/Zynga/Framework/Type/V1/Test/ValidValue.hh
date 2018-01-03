<?hh // strict

namespace Zynga\Framework\Type\V1\Test;

class ValidValue {
  private mixed $_input;
  private mixed $_expected;

  public function __construct(mixed $input, mixed $expected) {
    $this->_input = $input;
    $this->_expected = $expected;
  }

  public function getInputValue(): mixed {
    return $this->_input;
  }

  public function getExpectedValue(): mixed {
    return $this->_expected;
  }

}
