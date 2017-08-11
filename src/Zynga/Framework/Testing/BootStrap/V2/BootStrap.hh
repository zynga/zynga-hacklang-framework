<?hh // strict

namespace Zynga\Framework\Testing\BootStrap\V2;

use Zynga\Framework\Environment\CodePath\V1\CodePath;

class BootStrap {

  public static function init(): void {
    self::initTestErrorLog();
  }

  public static function getFileSystemRoot(): string {
    return CodePath::getRoot().'/tests';
  }

  public static function getTestErrorLog(): string {
    $errorLogFile = self::getFileSystemRoot().'/php.log';
    return $errorLogFile;
  }

  public static function initTestErrorLog(): void {

    // --
    // We setup a different location for all error_log content to goto.
    // --
    $testPhpLog = self::getTestErrorLog();

    if (!is_file($testPhpLog)) {
      touch($testPhpLog, 0755);
    }

    ini_set('error_log', $testPhpLog);
    ini_set('error_reporting', E_ALL);

    ini_set('log_errors', true);
    ini_set('hhvm.debug.server_error_message', true);
  }

}
