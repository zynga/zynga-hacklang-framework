<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Argument\Base;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;

class StringArgument extends Base {
  private StringBox $_value;

  public function __construct(
    string $name,
    string $description,
    bool $required,
  ) {
    parent::__construct($name, $description, $required);
    $this->_value = new StringBox();
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

  public function get(): string {
    return $this->_value->get();
  }

  public function getArgumentUsage(): string {
    return '--'.$this->getName().'=.*';
  }

  public function getArgumentRegex(): string {
    return preg_quote('--'.$this->getName().'=').'(.*)';
  }

  public function parseArgument(string $argument): bool {
    try {
      $argRegex = $this->getArgumentRegex();
      $m_args = array();
      if (preg_match('/'.$argRegex.'/', $argument, $m_args)) {
        $value = $m_args[1];
        $q_args = array();
        if (preg_match('/^(?:\'|")(.*)(?:\'|")$/', $value, $q_args)) {
          $value = $q_args[1];
        }
        $this->set($value);
      }
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

}
