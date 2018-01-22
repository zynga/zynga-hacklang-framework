<?hh // strict

namespace Zynga\Framework\Performance\V1;

class XHProfiler {
  private static $_profileDir;
  private static $_isProfiling;
  private static $_sampleHits;
  private static $_sampleTime;

  public static function error($string) {
    // echo "ERROR: $string\n";
    error_log($string);
  }

  public static function isXhProfileAvailable() {
    if ( function_exists('xhprof_enable') ) {
      return true;
    }
    return false;
  }

  public static function isXhProfileEnabled() {
    if ( getenv('xhprof.enable') == true ) {
      return true;
    }
    return false;
  }

  // Sample by time in secs.
  public static function isXhProfileSampleTime() {
    if ( !isset(self::$_sampleTime) ) {
      self::$_sampleTime = getenv('xhprof.sample.time');
    }
    if ( is_numeric(self::$_sampleTime) === false ) {
      return false;
    }
    return self::$_sampleTime;
  }

  // Sample by hits ratio, one every 'n' requests
  public static function isXhProfileSampleHits() {
    if ( ! isset(self::$_sampleHits) ) {
      self::$_sampleHits = getenv('xhprof.sample.hits');
    }
    if ( is_numeric(self::$_sampleHits) === false ) {
      return false;
    }
    return self::$_sampleHits;
  }

  public static function getHostname() {
    $hostname = null;
    if ( isset($_SERVER['HTTP_HOST']) ) {
      $hostname = $_SERVER['HTTP_HOST'];
      if (strpos($hostname,'slice') !== false || strpos($hostname,'localhost') !== false ) {
        return $hostname;
      }
    }
    return gethostname();
  }

  public static function getTmpDirFromPath() {
    $user = $_SERVER['USER'];
    $thisDir = dirname(__FILE__);
    $gitCurrentBranchCmd = "cd $thisDir ; git branch | grep \* | cut -d ' ' -f2 2>&1";
    $gitCurrentBranch = exec($gitCurrentBranchCmd);

    $pregs = null;
    if ( preg_match('/feature_(.*)_(.*)/', $gitCurrentBranch, $pregs) ) {
      $gitCurrentBranch = $pregs[1];
    }

    // slice-jorcutt-sfxevents.dev2.poker.zynga.com
    $dir = 'slice-' . strtolower($user) . '-' . strtolower($gitCurrentBranch) . '.dev2.poker.zynga.com';

    return $dir;
  }

  public static function getProfileDir() {

    if ( isset(self::$_profileDir) ) {
      return self::$_profileDir;
    }

    $tmpDir = '/tmp/xhprof/';

    $hostname = self::getHostname();

    // HAX, not all the code is available in this context, since it's a most likely a
    // cli.
    if ( $hostname == 'poker-dev-web-02.usw2.zynga.com' ) {
      $tmpDir .= self::getTmpDirFromPath();
    } else if ( $hostname !== null ) {
      $tmpDir .= '/' . $hostname;
    }

    if ( ! is_dir($tmpDir) ) {
      if ( @mkdir($tmpDir, 0755, true) !== true ) {
        self::$_profileDir = null;
        error_log('WARNING profileDirFailure=' . $tmpDir);
        return null;
      }
    }

    self::$_profileDir = $tmpDir;

    return self::$_profileDir;

  }

  public static function setupOutputDir() {

    $xhProfDir = self::getProfileDir();

    if ( $xhProfDir === null ) {
      self::error('WARNING - failed to turn on xhprofiling dir=' . $xhProfDir);
      return false;
    }

    if ( ini_get( 'hhvm.stats.enable_hot_profiler' ) ) {
      // This is hhvm, so don't bother trying to do ini_set / ini_get
      return true;
    }

    if ( ini_set('xhprof.output_dir', $xhProfDir) !== false ) {
      //self::error('XHProf ini=' . ini_get('xhprof.output_dir'));
      return true;
    }

    self::error('XHProf ini_set failed xhprof.output_dir is default=' . ini_get('xhprof.output_dir'));
    return false;

  }

  public static function startProfiling() {

    if ( self::isXhProfileAvailable() !== true ) {
      self::error('WARNING - you asked for xhprof profiling but the extension is not available.');
      return false;
    }

    if ( self::isXhProfileEnabled() !== true ) {
      return false;
    }

    if ( self::isXhProfileSampleTime() !== false ) {
      if ( (time() % self::$_sampleTime) !== 0 ) {
        return false;
      }
      //self::error("XHPROF: sampling by time matched");
    }

    if ( self::isXhProfileSampleHits() !== false ) {
      if ( mt_rand(1, self::$_sampleHits) != 1 ) {
        return false;
      }
      //self::error("XHPROF: sampling by hits matched");
    }

    // --
    // We have to raise the memory obsenely high to see everything that is happening.
    // --
    ini_set('memory_limit', '4096M');

    self::setupOutputDir();

    xhprof_enable(XHPROF_FLAGS_CPU + XHPROF_FLAGS_MEMORY);

    // self::error('STARTED_PROFILING');

    self::$_isProfiling = true;
    return true;

  }

  public static function stopProfiling($context) {

    if ( self::$_isProfiling !== true ) {
			// Currently stopProfiling is called from xhprofile_append script even if profiling was not started due to sampling, lever, etc.
      // To avoid noise, disabling the warnings for now till we fix this script.
      // self::error('WARNING - xhprofile was not started');
      return false;
    }

    self::setupOutputDir();

    $hostname = self::getHostname();

    if ( $hostname !== null ) {
      $context = $hostname . '-' . $context;
    }

    // --
    // xhprof capture writing time.
    // --
    $xhprof_data = xhprof_disable();

    $ROOT = ZY_CASINO_PATH . '/include/external/xhprof';

    require_once $ROOT . '/xhprof_lib/utils/xhprof_lib.php';
    require_once $ROOT . '/xhprof_lib/utils/xhprof_runs.php';

    $xhProfDir = self::getProfileDir();

    // self::error('xhProfDir=' . $xhProfDir . ' context=' . $context);
    $xhprof_runs = new XHProfRuns_Default($xhProfDir);

    $run_id = $xhprof_runs->save_run($xhprof_data, $context);
    //self::error('xhRunId=' . $run_id);

  }

}
