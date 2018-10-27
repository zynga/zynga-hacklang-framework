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

  public function simpleTryCatch(string $foo): bool {
    try {
      if ($foo == 'bar') {
        return true;
      }
      throw new Exception('must be the bar');
    } catch (Exception $e) {
      return false;
    }
  }

  private bool $didFinally = false;

  public function resetFinally(): void {
    $this->didFinally = false;
  }

  public function didFinally(): bool {
    return $this->didFinally;
  }

  public function moreComplexTryCatch(string $foo): bool {
    try {
      if ( $foo == 'bar' ) {
        return true;
      }
      throw new Exception('must be the bar');
    } catch ( Exception $e ) {
      return false;
    } finally {
      $this->didFinally = true;
    }
  }

}
