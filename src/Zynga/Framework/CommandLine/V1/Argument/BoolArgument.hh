<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Argument\Base;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Type\V1\BoolBox;

class BoolArgument extends Base {
  private BoolBox $_value;

  public function __construct(
    string $name,
    string $description,
    bool $required,
  ) {
    parent::__construct($name, $description, $required);
    $this->_value = new BoolBox();
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

  public function get(): bool {
    return $this->_value->get();
  }

  public function createOnArgumentStrings(): Vector<string> {
    $vec = Vector {};
    $vec[] = '--'.$this->getName();
    $vec[] = '--with-'.$this->getName();
    return $vec;
  }

  public function createOffArgumentStrings(): Vector<string> {
    $vec = Vector {};
    $vec[] = '--no-'.$this->getName();
    $vec[] = '--without-'.$this->getName();
    return $vec;
  }

  public function parseArgument(string $argument): bool {
    $onStrings = $this->createOnArgumentStrings();
    foreach ($onStrings as $onString) {
      if ($argument === $onString) {
        $this->set(true);
        return true;
      }
    }
    $offStrings = $this->createOffArgumentStrings();
    foreach ($offStrings as $offString) {
      if ($argument === $offString) {
        $this->set(false);
        return true;
      }
    }
    return true;
  }

  public function getArgumentUsage(): string {
    $onStrings = $this->createOnArgumentStrings();
    $offStrings = $this->createOffArgumentStrings();
    $usage =
      implode('|', $onStrings->toArray()).
      '|'.
      implode('|', $offStrings->toArray());
    return $usage;
  }

}
