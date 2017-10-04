<?hh // strict

namespace Zynga\Framework\Database\V2\Test\Config\Mock\Base;

use Zynga\Framework\Database\V2\Config\Mock\Base as ConfigBase;

class NoUserName extends ConfigBase {

  public function init(): bool {
    return parent::init();
  }

}
