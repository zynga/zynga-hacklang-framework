<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Interfaces\ArgumentInterface;
use Zynga\Framework\CommandLine\V1\Interfaces\ArgumentContainerInterface;

use Zynga\Framework\CommandLine\V1\Exceptions\UnableToFindArgumentException;
use Zynga\Framework\CommandLine\V1\Exceptions\MissingArgumentsException;
use
  Zynga\Framework\CommandLine\V1\Exceptions\UnableToReadScriptArgumentsException
;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Exception;

class Container implements ArgumentContainerInterface {

  private Map<string, ArgumentInterface> $_arguments;

  public function __construct() {
    $this->_arguments = Map {};
  }

  public function getArguments(): Vector<ArgumentInterface> {
    $vec = Vector {};
    foreach ($this->_arguments as $argumentName => $argument) {
      $vec[] = $argument;
    }
    return $vec;
  }

  public function addArgument(ArgumentInterface $arg): bool {
    $argName = $arg->getName();
    if ($this->_arguments->containsKey($argName) !== true) {
      $this->_arguments[$argName] = $arg;
    }
    return true;
  }

  public function getArgument(string $name): ArgumentInterface {
    foreach ($this->_arguments as $argument) {
      if ($argument->getName() == $name) {
        return $argument;
      }
    }
    throw new UnableToFindArgumentException('name='.$name);
  }

  public function parse(): bool {

    try {

      $this->parseArguments();

      $this->requiredOptionsSet();

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function requiredOptionsSet(): bool {

    $missingArguments = array();

    foreach ($this->_arguments as $argument) {
      if ($argument->isRequired() === true &&
          $argument->hasValue() !== true) {
        $missingArguments[] = $argument->getName();
      }
    }

    if (count($missingArguments) > 0) {
      throw new MissingArgumentsException(
        'missingArguments='.implode(', ', $missingArguments),
      );
    }

    return true;

  }

  public function parseArguments(): bool {

    try {

      $argv = SuperGlobals::getScriptCommandLineArguments();

      // unable to read the super globals.
      if (!is_array($argv)) {
        throw new UnableToReadScriptArgumentsException(
          'argv fails array test',
        );
      }

      // no work with no aguments.
      if (count($argv) === 0) {
        return true;
      }

      foreach ($this->_arguments as $argument) {
        foreach ($argv as $arg) {
          $argument->parseArgument($arg);
        }
      }

      return true;
    } catch (Exception $e) {
      throw $e;
    }

  }

}
