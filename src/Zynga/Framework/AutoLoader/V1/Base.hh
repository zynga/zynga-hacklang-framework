<?hh

// --
// JEO: This file cannot be strict due to require_once being fired conditionally.
// --
namespace Zynga\Framework\AutoLoader\V1;

use \Exception;

abstract class Base {

  const string NAMESPACE_SEPERATOR = '\\';
  const string DIRECTORY_SEPERATOR = '/';

  private bool $_debug;
  private string $_debugTarget;

  public function __construct() {
    $this->_debug = false;
    $this->_debugTarget = 'stdout';
  }

  public function enableDebug(): bool {
    $this->_debug = true;
    return true;
  }

  public function disableDebug(): bool {
    $this->_debug = false;
    return true;
  }

  public function getDebug(): bool {
    return $this->_debug;
  }

  public function setDebugTarget(string $target): bool {
    if ($target == 'stdout' || $target == 'error_log') {
      $this->_debugTarget = $target;
      return true;
    }
    return false;
  }

  public function getDebugTarget(): string {
    return $this->_debugTarget;
  }

  public function load(string $className): bool {
    return $this->loadClass($this->getFileSystemRoot(), $className);
  }

  abstract public function getFileSystemRoot(): string;

  public function convertClassNameToClassPath(string $className): string {

    // Convert the class name to safe file path name.
    $newPath = str_replace(
      self::NAMESPACE_SEPERATOR,
      self::DIRECTORY_SEPERATOR,
      $className,
    );

    return $newPath;

  }

  public function createFileSystemPath(
    string $path,
    string $className,
  ): string {
    $fsPath = '';
    $fsPath .= $path;
    $fsPath .= self::DIRECTORY_SEPERATOR;
    $fsPath .= $this->convertClassNameToClassPath($className);
    $fsPath .= ".hh";
    return $fsPath;
  }

  public function verifyClassLoaded(
    string $casinoPath,
    string $className,
  ): bool {
    if (class_exists($className) || interface_exists($className)) {
      $this->debugMessage(
        "loadClass className=$className casinoPath=$casinoPath - FOUND",
      );
      return true;
    }
    $this->debugMessage(
      "loadClass className=$className casinoPath=$casinoPath - MISS_NO_CLASS_OR_INTERFACE",
    );
    return false;
  }

  public function loadClass(string $path, string $className): bool {

    // Construct the casino path from the root + the dir safe class name.
    $casinoPath = $this->createFileSystemPath($path, $className);

    $this->debugMessage(
      "loadClass className=$className casinoPath=$casinoPath",
    );

    // attempt to include the casino path
    if (is_file($casinoPath)) {

      // JEO: This isn't needed anytmore.
      require_once ($casinoPath);

      return $this->verifyClassLoaded($casinoPath, $className);

    }

    $this->debugMessage(
      "loadClass className=$className casinoPath=$casinoPath - MISS_NO_FILE",
    );
    return false;

  }

  // Registers the autoloader to the environment, spl_autoload actually will de-dupe the requests if needed.
  public function register(): bool {
    $callback = $this->findRegisteredAutoLoader();
    if ($callback === null) {
      spl_autoload_register(array($this, 'load'));
      return true;
    }
    return false;
  }

  public function getClassName(): string {
    return get_class($this);
  }

  public function unregister(): bool {
    $callback = $this->findRegisteredAutoLoader();
    if ($callback !== null) {
      spl_autoload_unregister($callback);
      return true;
    }
    return false;
  }

  public function findRegisteredAutoLoader(): ?array {

    $className = $this->getClassName();

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
  public function debugMessage(string $message): void {

    // if ($this->_debug !== true) {
    //   return;
    // }

    $leader = $this->getClassName();

    if ($this->_debugTarget === 'stdout') {
      echo $leader.' '.$message."\n";
      return;
    }

    trigger_error($leader.' '.$message, E_USER_ERROR);
    return;

  }

}
