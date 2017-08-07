<?hh // strict

namespace Zynga\Framework\AutoLoader\V1\Test\Mock;

use Zynga\Framework\AutoLoader\V1\Base;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

class AutoLoader extends Base {

  public function getFileSystemRoot(): string {
    return CodePath::getRoot().'/tests/autoloader';
  }

}
