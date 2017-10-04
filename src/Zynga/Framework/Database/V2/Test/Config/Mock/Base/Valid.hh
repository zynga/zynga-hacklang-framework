<?hh // strict

namespace Zynga\Framework\Database\V2\Test\Config\Mock\Base;

use Zynga\Framework\Database\V2\Config\Mock\Base as ConfigBase;

class Valid extends ConfigBase {
  public function init(): bool {
    $this->setUserName('someusername');
    $this->setPassword('somepassword');
    $this->setPort(123);
    return parent::init();
  }
}
