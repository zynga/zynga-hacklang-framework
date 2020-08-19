<?hh // partial

namespace Zynga\Framework\Environment\SuperGlobals\V1;

class SuperGlobals {
  const string SCRIPT_FILE = 'SCRIPT_FILENAME';
  const string SCRIPT_URI = 'SCRIPT_URI';
  const string SCRIPT_ARGV = 'argv';
  const string SCRIPT_ARGC = 'argc';
  const string DEFAULT_HTTP_BODY_FILE = 'php://input';

  private static string $_bodyFilePath = self::DEFAULT_HTTP_BODY_FILE;

  public static function setScriptUri(string $uri): bool {
    $_SERVER[self::SCRIPT_URI] = $uri;
    return true;
  }

  public static function getScriptUri(): string {
    if (key_exists(self::SCRIPT_URI, $_SERVER)) {
      return $_SERVER[self::SCRIPT_URI];
    }
    return '';
  }

  public static function clearScriptUri(): bool {
    if (key_exists(self::SCRIPT_URI, $_SERVER)) {
      unset($_SERVER[self::SCRIPT_URI]);
      return true;
    }
    return false;
  }

  public static function setScriptName(string $value): bool {
    $_SERVER[self::SCRIPT_FILE] = $value;
    return true;
  }

  public static function getScriptName(): string {
    if (key_exists(self::SCRIPT_FILE, $_SERVER)) {
      return $_SERVER[self::SCRIPT_FILE];
    }
    return '';
  }

  public static function unsetScriptName(): bool {
    if (key_exists(self::SCRIPT_FILE, $_SERVER)) {
      unset($_SERVER[self::SCRIPT_FILE]);
    }
    return true;
  }

  public static function setScriptCommandLineArguments(
    array<string> $args,
  ): bool {
    $_SERVER[self::SCRIPT_ARGV] = $args;
    $_SERVER[self::SCRIPT_ARGC] = count($args);
    return true;
  }

  public static function getScriptCommandLineArguments(): mixed {
    if (key_exists(self::SCRIPT_ARGV, $_SERVER)) {
      return $_SERVER[self::SCRIPT_ARGV];
    }
    return null;
  }

  public static function clearScriptCommandLineArguments(): bool {
    if (key_exists(self::SCRIPT_ARGV, $_SERVER)) {
      unset($_SERVER[self::SCRIPT_ARGV]);
    }
    if (key_exists(self::SCRIPT_ARGC, $_SERVER)) {
      unset($_SERVER[self::SCRIPT_ARGC]);
    }
    return true;
  }

  public static function unsetRequestPairing(string $key): bool {
    if (key_exists($key, $_REQUEST)) {
      unset($_REQUEST[$key]);
    }
    return true;
  }

  public static function clearRequest(): bool {
    $_REQUEST = array();
    return true;
  }

  public static function setRequestPairing(string $key, mixed $value): bool {
    $_REQUEST[$key] = $value;
    return true;
  }

  public static function getServerAsMap(): Map<string, mixed> {
    return new Map($_SERVER);
  }

  public static function getGetAsMap(): Map<string, mixed> {
    return new Map($_GET);
  }

  public static function getPostAsMap(): Map<string, mixed> {
    return new Map($_POST);
  }

  public static function getRequestAsMap(): Map<string, mixed> {
    return new Map($_REQUEST);
  }

  public static function getFilesAsMap(): Map<string, mixed> {
    return new Map($_FILES);
  }

  public static function getBodyFilePath(): string {
    return self::$_bodyFilePath;
  }

  public static function setBodyFilePath(string $file): bool {
    self::$_bodyFilePath = $file;
    return true;
  }

  public static function getRequestBodyAsMap(): Map<string, mixed> {

    $rawBody = file_get_contents(self::$_bodyFilePath);

    $bodyAsArray = json_decode($rawBody, true);

    if (is_array($bodyAsArray) === true) {
      return new Map($bodyAsArray);
    }

    $blank = Map {};

    return $blank;

  }

}
