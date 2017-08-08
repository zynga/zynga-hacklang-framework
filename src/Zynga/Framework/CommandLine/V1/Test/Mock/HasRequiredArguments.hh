<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Test\Mock;

use Zynga\Framework\CommandLine\V1\Base;
use Zynga\Framework\CommandLine\V1\Argument\StringArgument;

class HasRequiredArguments extends Base {

  public function setupArguments(): bool {

    $testArgName = 'monkey';
    $testArgDesc = 'monkeys like to be strings';
    $testArgReq = true;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->args()->addArgument($arg);

    $testArgName = 'monkey22';
    $testArgDesc = 'monkeys like to be strings22';
    $testArgReq = false;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->args()->addArgument($arg);

    $testArgName = 'reqAndHasValue';
    $testArgDesc = 'monkeys like to be strings';
    $testArgReq = true;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->args()->addArgument($arg);

    return true;
  }

  public function handleRun(): int {
    return 0;
  }

  public function getTime(): int {
    // hardcoded to JEO: dob
    return 236563200;
  }

  // NOOP the output.
  public function error(string $messsage): void {}

}
