<?hh // strict

namespace Zynga\Framework\Environment\CodePath\V1;

class CodePath {
  private static string $_root = '';

  public static function setRoot(string $path): bool {
    self::$_root = $path;
    return true;
  }

  public static function getRoot(): string {
    return self::$_root;
  }

}
