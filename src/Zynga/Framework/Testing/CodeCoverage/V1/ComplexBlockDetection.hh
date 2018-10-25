<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use \Exception;

class ComplexBlockDetection {

  public function simpleIf(string $foo): bool {

    if ($foo == 'bar') {

      return true;

    }

    return false;

  }

  public function moreComplexIf(string $foo): bool {

    if ($foo == 'bar') {
      return true;
    } else if ($foo == 'baz') {
      return true;
    } else {
      return false;
    }

  }

}
