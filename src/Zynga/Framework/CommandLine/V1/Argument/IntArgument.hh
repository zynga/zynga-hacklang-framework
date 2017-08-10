<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Argument\Base;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Type\V1\Int64Box;

class IntArgument extends Base {
  private Int64Box $_value;

  public function __construct(
    string $name,
    string $description,
    bool $required,
  ) {
    parent::__construct($name, $description, $required);
    $this->_value = new Int64Box();
  }

  public function set(mixed $value): bool {
    try {
      $this->_value->set($value);
      $this->setHasValue(true);
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function get(): int {
    return $this->_value->get();
  }

  public function getArgumentUsage(): string {
    return '--'.$this->getName().'=\d+';
  }

}
