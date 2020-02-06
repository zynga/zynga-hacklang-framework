<?hh // strict

namespace Zynga\Framework\AutoLoader\V2\ZyngaFrameworkAutoloader;

class Path {
  private string $_startsWith;
  private string $_path;
  private Vector<string> $_exts;

  public function __construct(
    string $startsWith,
    string $path,
    Vector<string> $exts,
  ) {
    $this->_startsWith = $startsWith;
    $this->_path = $path;
    $this->_exts = $exts;
  }

  public function getStartsWith(): string {
    return $this->_startsWith;
  }

  public function doesClassMatchStartsWith(string $class): bool {
    if (strpos($class, $this->_startsWith) === 0) {
      return true;
    }
    return false;
  }

  public function getPath(): string {
    return $this->_path;
  }

  public function getExtensions(): Vector<string> {
    return $this->_exts;
  }

}
