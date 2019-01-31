<?hh // strict

namespace Zynga\Framework\Environment\CodePath\V1;

/**
 * CodePath is intended to provide pathing information for your application.
 * It is recommended to use absolute paths for better performance on file inclusion.
 */
class CodePath {
  private static string $root = '';
  private static string $externalLibraryPath = '';

  public static function setRoot(string $path): bool {
    self::$root = $path;
    return true;
  }

  public static function getRoot(): string {
    return self::$root;
  }

  public static function resetPaths(): void {
    self::$root = '';
    self::$externalLibraryPath = '';
  }

  public static function getExternalLibraryPath(): string {
    return self::$externalLibraryPath;
  }

  public static function setExternalLibraryPath(string $path): void {
    self::$externalLibraryPath = $path;
  }
}
