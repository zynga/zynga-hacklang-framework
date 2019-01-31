<?hh // partial

namespace Zynga\Framework\Performance\V1;

use Zynga\Framework\Environment\CodePath\V1\CodePath;

require_once
  CodePath::getExternalLibraryPath().
  '/xhprof/xhprof_lib/utils/xhprof_lib.php'
;
require_once
  CodePath::getExternalLibraryPath().
  '/xhprof/xhprof_lib/utils/xhprof_runs.php'
;

use \XHProfRuns_Default;

/**
 * Before running xhprofiler with your application, be sure you have set the
 * path to the library as noted in the above requires.
 */
class XHProfiler {
  private static string $_profileDir = '';
  private static bool $_isProfiling = false;

  public static function error(string $string): bool {
    error_log($string);
    return true;
  }

  public static function isXhProfileAvailable(): bool {
    if (function_exists('xhprof_enable')) {
      return true;
    }
    return false;
  }

  public static function isXhProfileEnabled(): bool {
    if (getenv('xhprof.enable') == true) {
      return true;
    }
    return false;
  }

  public static function setProfileDir(string $dir): bool {

    if (!is_dir($dir)) {
      mkdir($dir, 0755);
    }

    if (is_dir($dir) === true && is_writeable($dir) === true) {
      self::$_profileDir = $dir;
      return true;
    }

    return false;

  }

  public static function getProfileDir(): string {

    return self::$_profileDir;

  }

  public static function startProfiling(): bool {

    if (self::isXhProfileAvailable() !== true) {
      self::error(
        'WARNING - you asked for xhprof profiling but the extension is not available.',
      );
      return false;
    }

    if (self::isXhProfileEnabled() !== true) {
      return false;
    }

    xhprof_enable(XHPROF_FLAGS_CPU + XHPROF_FLAGS_MEMORY);

    self::error('STARTED_PROFILING');

    self::$_isProfiling = true;
    return true;

  }

  public static function stopProfiling(string $context): bool {

    if (self::$_isProfiling !== true) {
      // Currently stopProfiling is called from xhprofile_append script even if profiling was not started due to sampling, lever, etc.
      // To avoid noise, disabling the warnings for now till we fix this script.
      self::error('WARNING - xhprofile was not started');
      return false;
    }

    // --
    // xhprof capture writing time.
    // --
    $xhprof_data = xhprof_disable();

    $xhProfDir = self::getProfileDir();

    self::error('xhProfDir='.$xhProfDir.' context='.$context);
    $xhprof_runs = new XHProfRuns_Default($xhProfDir);

    $run_id = $xhprof_runs->save_run($xhprof_data, $context);
    self::error('xhRunId='.$run_id);

    return true;

  }

}
