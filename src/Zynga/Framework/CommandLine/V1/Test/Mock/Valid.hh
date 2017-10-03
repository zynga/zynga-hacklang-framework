<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Test\Mock;

use Zynga\Framework\CommandLine\V1\Base;

class Valid extends Base {

  public function handleRun(): int {
    return 0;
  }

  public function getTime(): int {
    // hardcoded to JEO: dob
    return 236563200;
  }

}
