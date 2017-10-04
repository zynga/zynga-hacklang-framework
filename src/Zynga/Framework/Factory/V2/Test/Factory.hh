<?hh // strict

namespace Zynga\Framework\Factory\V2\Test;

use Zynga\Framework\Factory\V2\Base as FactoryBase;

class Factory extends FactoryBase {
  public static function getClassRoot(): string {
    return 'Zynga\Framework\Factory\V2\Test';
  }
}
