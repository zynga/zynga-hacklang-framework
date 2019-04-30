<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3;

use Zynga\Framework\ShardedDatabase\V3\Exceptions\UnknownSocialNetworkId;
use Zynga\Framework\ShardedDatabase\V3\Mock\BadSocialNetworkMapInfo;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;

class InfoTest extends TestCase {
  public function testInvalidSocialNetwork(): void {
    $this->expectException(UnknownSocialNetworkId::class);
    BadSocialNetworkMapInfo::getDatabaseSchemaForSocialNetworkId(SnidBoxFactory::facebook());
  }
}
