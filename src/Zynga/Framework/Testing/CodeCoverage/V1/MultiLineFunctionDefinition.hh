<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use \Exception;

class MultiLineFunctionDefinition {
  public function doSomethingGreat(
    string $someParam,
    string $anotherParam,
    string $yetMoreParams,
  ): bool {
    return true;
  }
}
