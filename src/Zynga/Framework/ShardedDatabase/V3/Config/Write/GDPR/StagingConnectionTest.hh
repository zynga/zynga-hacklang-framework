<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Write\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionReadDeleteBaseTest
;
use Zynga\Framework\ShardedDatabase\V3\Info;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class StagingConnectionTest extends ConnectionReadDeleteBaseTest {

  public function getDriverName(): string {
    return 'Write\GDPR';
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
