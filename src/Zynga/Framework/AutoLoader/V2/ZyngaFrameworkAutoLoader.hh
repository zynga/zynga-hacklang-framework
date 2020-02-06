<?hh

// --
// JEO: This file cannot be strict due to require_once being fired conditionally.
// --
namespace Zynga\Framework\AutoLoader\V2;

use
  Zynga\Framework\AutoLoader\V2\ZyngaFrameworkAutoloader\Path as ZyngaFrameworkAutoLoader_Path
;
use Zynga\Framework\AutoLoader\V2\ZyngaFrameworkAutoLoader\Cache;

use \Exception;

final class ZyngaFrameworkAutoLoader {

  const string PSR4_SEPERATOR = '_';
  const string NAMESPACE_SEPERATOR = '\\';
  const string DIRECTORY_SEPERATOR = '/';

  private static bool $_cacheFounds = true;
  private static int $_cacheFoundTTL = 86400;

  private static bool $_cacheMisses = true;
  private static int $_cacheMissTTL = 86400;

  private static bool $_debug = false;
  private static string $_debugTarget = 'stdout';
  private static Vector<ZyngaFrameworkAutoLoader_Path> $_paths = Vector {};

  public static function enableDebug(): bool {
    self::$_debug = true;
    return true;
  }

  public static function disableDebug(): bool {
    self::$_debug = false;
    return true;
  }

  public static function getDebug(): bool {
    return self::$_debug;
  }

  public static function setDebugTarget(string $target): bool {
    if ($target == 'stdout' || $target == 'error_log') {
      self::$_debugTarget = $target;
      return true;
    }
    return false;
  }

  public static function getDebugTarget(): string {
    return self::$_debugTarget;
  }

  public static function setCacheFounds(bool $tf): bool {
    self::$_cacheFounds = $tf;
    return true;
  }

  public static function getCacheFounds(): bool {
    return self::$_cacheFounds;
  }

  public static function setCacheFoundTTL(int $ttl): bool {
    if ($ttl > 0) {
      self::$_cacheFoundTTL = $ttl;
      return true;
    }
    return false;
  }

  public static function getCacheFoundTTL(): int {
    return self::$_cacheFoundTTL;
  }

  public static function setCacheMisses(bool $tf): bool {
    self::$_cacheMisses = $tf;
    return true;
  }

  public static function getCacheMisses(): bool {
    return self::$_cacheMisses;
  }

  public static function setCacheMissTTL(int $ttl): bool {
    self::$_cacheMissTTL = $ttl;
    return true;
  }

  public static function getCacheMissTTL(): int {
    return self::$_cacheMissTTL;
  }

  public static function addPath(
    string $startsWith,
    string $path,
    Vector<string> $exts,
  ): bool {
    if (is_readable($path)) {
      $path = new ZyngaFrameworkAutoLoader_Path($startsWith, $path, $exts);
      self::$_paths->add($path);
      return true;
    }
    return false;
  }

  public static function load(string $className): bool {

    if (self::$_cacheFounds === true) {

      $cachedValue = Cache::getFromCache($className);

      self::debugMessage(
        "getFromCache className=$className cachedValue=".
        json_encode($cachedValue),
      );

      if (is_string($cachedValue)) {
        if (self::$_cacheMisses === true && $cachedValue == 'NOT-FOUND') {
          // this item was not found, and we shouldn't try to hunt for it.
          self::debugMessage(
            'loaded - CACHE HIT, but NOT-FOUND file cached.',
          );
          return false;
        }
        self::debugMessage('loaded - CACHE HIT');
        return self::loadFromDisk($className, $cachedValue)[0];
      }

    }

    foreach (self::$_paths as $path) {
      if ($path->doesClassMatchStartsWith($className) !== true) {
        continue;
      }
      foreach ($path->getExtensions() as $ext) {
        $foundClass = self::loadClass($path->getPath(), $className, $ext);
        if ($foundClass[0] === true) {
          $classPath = $foundClass[1];
          self::debugMessage(
            "storeIncache ccasinoPath=$classPath lassName=$className",
          );
          if (self::$_cacheFounds === true) {
            Cache::storeInCache(
              $className,
              $classPath,
              self::$_cacheFoundTTL,
            );
          }
          return $foundClass[0];
        }
      }
    }

    // we also cache misses to prevent disk-io that isn't needed, much lower ttl.
    self::debugMessage(
      "storeIncache ccasinoPath=NOT-FOUND lassName=$className",
    );

    if (self::$_cacheMisses === true) {
      Cache::storeInCache($className, 'NOT-FOUND', self::$_cacheMissTTL);
    }

    return false;
  }

  // Perform a psr 4 path conversion, if it's a namespace.
  public static function convertClassNameToClassPath(
    string $className,
  ): string {

    // Convert the class name to safe file path name.
    $newPath = str_replace(
      self::NAMESPACE_SEPERATOR,
      self::DIRECTORY_SEPERATOR,
      $className,
    );

    // Attempt to convert _ to slashes, if found within the target
    if (strpos($newPath, self::PSR4_SEPERATOR) !== false) {
      $newPath = str_replace(
        self::PSR4_SEPERATOR,
        self::DIRECTORY_SEPERATOR,
        $newPath,
      );
    }

    return $newPath;

  }

  public static function createFileSystemPath(
    string $path,
    string $className,
    string $extension,
  ): string {
    return sprintf(
      '%s%s%s.%s',
      $path,
      self::DIRECTORY_SEPERATOR,
      self::convertClassNameToClassPath($className),
      $extension,
    );
  }

  public static function verifyClassLoaded(
    string $casinoPath,
    string $className,
  ): bool {
    if (class_exists($className) || interface_exists($className)) {
      self::debugMessage(
        "loadClass className=$className casinoPath=$casinoPath - FOUND",
      );
      return true;
    }
    self::debugMessage(
      "loadClass className=$className casinoPath=$casinoPath - MISS_NO_CLASS_OR_INTERFACE",
    );
    return false;
  }

  public static function loadClass(
    string $path,
    string $className,
    string $ext,
  ): (bool, string) {

    // Construct the casino path from the root + the dir safe class name.
    $casinoPath = self::createFileSystemPath($path, $className, $ext);

    return self::loadFromDisk($className, $casinoPath);
  }

  public static function loadFromDisk(
    $className,
    $casinoPath,
  ): (bool, string) {

    self::debugMessage(
      "loadFromDisk className=$className casinoPath=$casinoPath",
    );

    // attempt to include the casino path
    if (is_file($casinoPath)) {

      // JEO: This isn't needed anytmore.
      require_once ($casinoPath);

      if (self::verifyClassLoaded($casinoPath, $className) === true) {
        return tuple(true, $casinoPath);
      }

    }

    self::debugMessage(
      "loadClass className=$className casinoPath=$casinoPath - MISS_NO_FILE",
    );

    return tuple(false, 'NOT-FOUND');

  }

  // Registers the autoloader to the environment, spl_autoload actually will de-dupe the requests if needed.
  public static function register(): bool {
    $callback = self::findRegisteredAutoLoader();
    if ($callback === null) {
      spl_autoload_register(array(__CLASS__, 'load'));
      return true;
    }
    return false;
  }

  public static function getClassName(): string {
    return __CLASS__;
  }

  public static function unregister(): bool {
    $callback = self::findRegisteredAutoLoader();
    if ($callback !== null) {
      spl_autoload_unregister($callback);
      return true;
    }
    return false;
  }

  public static function findRegisteredAutoLoader(): ?array {

    $className = self::getClassName();

    $foundAutoLoader = false;

    $autoloaders = spl_autoload_functions();

    // --
    // JEO: You could really stretch and try to fix the hhvm vs codecoverage issue here
    // but the reality is that we're talking about a very odd state, where the
    // inclusion tree is not initalized at all, so therefor how did you load this file?
    // --
    //if ($autoloaders === false) {
    //  return null;
    //}

    if (is_array($autoloaders)) {
      foreach ($autoloaders as $autoloader) {
        if (is_array($autoloader) &&
            is_object($autoloader[0]) &&
            get_class($autoloader[0]) == $className) {
          return $autoloader;
        }
      }
    }

    return null;

  }

  /**
   *
   * JEO: Our phpunit will cry/whine if we have output out of this function.
   *  both stdout | stderr will emit failures.
   *
   */
  public static function debugMessage(string $message): void {

    if (self::$_debug !== true) {
      return;
    }

    $leader = self::getClassName();

    if (self::$_debugTarget === 'stdout') {
      echo $leader.' '.$message."\n";
      return;
    }

    trigger_error($leader.' '.$message, E_USER_ERROR);
    return;

  }

}
