<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use
  Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFeofFalseOnce as BaseManager
;

class ManagerWithFeofFalseOnceFcloseFails extends BaseManager {

  <<__Override>>
  public function fclose(resource $handle): bool {
    return false;
  }

}
