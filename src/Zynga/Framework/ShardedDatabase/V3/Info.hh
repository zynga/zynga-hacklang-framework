<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3;

use Zynga\Framework\ShardedDatabase\V3\Exceptions\UnknownSocialNetworkId;

use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class Info {
  const string SCHEMA_FACEBOOK = 'Poker';

  const string SCHEMA_SNAPI = 'PokerSnapi';

  const string SCHEMA_GUEST = 'PokerGuest';

  const string SCHEMA_SECURE_GUEST = 'PokerSnapi';

  public static function getSocialNetworkToSchemaMap(): Map<int, string> {
    $map = Map {};

    $map[SnidBoxFactory::facebook()->get()] = self::SCHEMA_FACEBOOK;
    $map[SnidBoxFactory::snapi()->get()] = self::SCHEMA_SNAPI;
    $map[SnidBoxFactory::guest()->get()] = self::SCHEMA_GUEST;
    $map[SnidBoxFactory::secureGuest()->get()] = self::SCHEMA_SECURE_GUEST;

    return $map;
  }

  public static function getSocialNetworkIds(): Vector<SnidBox> {
    $vec = Vector {};
    $vec[] = SnidBoxFactory::facebook();
    $vec[] = SnidBoxFactory::snapi();
    $vec[] = SnidBoxFactory::guest();
    $vec[] = SnidBoxFactory::secureGuest();
    return $vec;
  }

  public static function getSampleUserIds(): Vector<UidBox> {
    $vec = Vector {
      new UidBox(16),
      new UidBox(17),
      new UidBox(18),
      new UidBox(19),
      new UidBox(20),
      new UidBox(21),
      new UidBox(22),
      new UidBox(23),
      new UidBox(24),
      new UidBox(25),
      new UidBox(26),
      new UidBox(27),
      new UidBox(28),
      new UidBox(29),
      new UidBox(30),
      new UidBox(31),
    };
    return $vec;
  }

  public static function getDatabaseSchemaForSocialNetworkId(SnidBox $sn): string {
    $map = static::getSocialNetworkToSchemaMap();
    if ($map->containsKey($sn->get())) {
      return $map[$sn->get()];
    }

    throw new UnknownSocialNetworkId('sn='.$sn);
  }

}
