<?hh // strict

namespace Zynga\Framework\Environment\CodePath\V1;

use Zynga\Framework\Environment\CodePath\V1\Exceptions\PathNotFoundException;

/**
 * CodePath is intended to provide pathing information for your application.
 * It is recommended to use absolute paths for better performance on file inclusion.
 */
class CodePath {
  private static string $root = '';
  private static ?Map<string, string> $libraryPathMap = null;

  public static function setRoot(string $path): bool {
    self::$root = $path;
    return true;
  }

  public static function getRoot(): string {
    return self::$root;
  }

  public static function resetPaths(): void {
    self::$root = '';
    self::$libraryPathMap = null;
  }

  public static function getLibraryPath(string $library): string {
    $localPathMap = self::$libraryPathMap;

    if ($localPathMap !== null && $localPathMap->contains($library)) {
      return $localPathMap[$library];
    }

    throw new PathNotFoundException(
      'Library path not found. Library='.
      $library.
      ' knownLibraries='.
      json_encode($localPathMap),
    );
  }

  public static function setLibraryPath(string $library, string $path): void {
    if (self::$libraryPathMap === null) {
      self::$libraryPathMap = Map {};
    }
    self::$libraryPathMap[$library] = $path;
  }

  public static function getLibraryPathMap(): ?Map<string, string> {
    return self::$libraryPathMap;
  }

  public static function setLibraryPathMap(
    ?Map<string, string> $libraryPathMap,
  ): void {
    self::$libraryPathMap = $libraryPathMap;
  }
}
