<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Write;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionReadWriteBaseTest
;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\ShardedDatabase\V3\Info;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class StagingConnectionTest extends ConnectionReadWriteBaseTest {

  public function getDriverName(): string {
    return 'Write';
  }

  public function getEnvironment(): int {
    return DevelopmentMode::STAGING;
  }

  public function getSupportedSocialNetworks(): Vector<SnidBox> {
    return Info::getSocialNetworkIds();
  }

  public function getTestUserIds(): Vector<UidBox> {
    return Info::getSampleUserIds();
  }

  public function getSchemaForTests(): string {
    return 'Poker';
  }

  public function getTableNameForTests(): string {
    return 'Poker.phpunit';
  }

}
