<?hh // strict

namespace Zynga\Framework\Database\V2\Test\Config\Mock\Base;

use Zynga\Framework\Database\V2\Config\Mock\Base as ConfigBase;

class NoPassword extends ConfigBase {
  public function init(): bool {
    $this->setUserName('someusername');
    return parent::init();
  }
}
