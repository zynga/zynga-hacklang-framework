<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Test\Mock;

use Zynga\Framework\CommandLine\V1\Base;
use Zynga\Framework\CommandLine\V1\Test\Mock\ExampleException;

class TossedException extends Base {

  public function handleRun(): int {
    throw new ExampleException('Ohnoz');
  }

  public function getTime(): int {
    // hardcoded to JEO: dob
    return 236563200;
  }

}
