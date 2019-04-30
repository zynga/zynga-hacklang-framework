<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Mock\Base as ConfigBase
;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class NoPassword extends ConfigBase {
  public function shardsInit(): bool {
    $this->addServer(new ConnectionDetails('someusername', '', 'locahost', 0));
    return true;
  }
}
