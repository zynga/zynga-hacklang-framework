<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Mock;

use Zynga\Framework\ShardedDatabase\V3\Info as BaseInfo;
use Zynga\Type\V1\SnidBox;
use Zynga\Type\V1\UidBox;

class BadSocialNetworkMapInfo extends BaseInfo {
    public static function getSocialNetworkToSchemaMap(): Map<int, string> {
      $map = Map {};
      $map[0] = self::SCHEMA_FACEBOOK;
      return $map;
    }
}
