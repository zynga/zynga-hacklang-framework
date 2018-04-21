<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Mock;

use Zynga\Framework\IO\Disk\V1\Manager as BaseManager;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

class ManagerWithFailedToDeleteFile extends BaseManager {

  private static ?DiskIOManagerInterface $instance;

  <<__Override>>
  public static function instance(): DiskIOManagerInterface {
    if (self::$instance === null) {
      self::$instance = new self();
    }
    invariant(self::$instance !== null, '$instance was null');

    return self::$instance;
  }

  <<__Override>>
  public function checkOrCreatePath(string $path, int $permissions): bool {
    return true;
  }

  <<__Override>>
  public function deleteFile(string $path): bool {
    return false;
  }

  <<__Override>>
  public function writeFile(
    string $fileName,
    string $dataToWrite,
    int $permissions,
    bool $appendIfExists,
  ): void {}

  <<__Override>>
  public function bzip2(string $in, string $out): void {}

}
