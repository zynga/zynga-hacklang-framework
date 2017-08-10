<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Interfaces\ArgumentInterface;
use Zynga\Framework\Exception\V1\Exception;

abstract class Base implements ArgumentInterface {

  private string $_name;
  private string $_description;
  private bool $_required;
  private bool $_hasValue;

  public function __construct(
    string $name,
    string $description,
    bool $required,
  ) {
    $this->_name = $name;
    $this->_description = $description;
    $this->_required = $required;
    $this->_hasValue = false;
  }

  public function getName(): string {
    return $this->_name;
  }

  public function getDescription(): string {
    return $this->_description;
  }

  public function isRequired(): bool {
    return $this->_required;
  }

  public function setHasValue(bool $val): bool {
    $this->_hasValue = $val;
    return true;
  }

  public function hasValue(): bool {
    return $this->_hasValue;
  }

  public function getArgumentRegex(): string {
    return preg_quote('--'.$this->getName().'=').'(.*)';
  }

  public function parseArgument(string $argument): bool {
    try {
      $argRegex = $this->getArgumentRegex();
      $m_args = array();
      if (preg_match('/'.$argRegex.'/', $argument, $m_args)) {
        $this->set($m_args[1]);
      }
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

}
