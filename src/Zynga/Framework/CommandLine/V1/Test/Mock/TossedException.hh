<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Test\Mock;

use Zynga\Framework\CommandLine\V1\Base;
use Zynga\Framework\Exception\V1\Exception;

class TossedException extends Base {

  public function handleRun(): int {
    throw new Exception('Ohnoz');
  }

  public function getTime(): int {
    // hardcoded to JEO: dob
    return 236563200;
  }

}
