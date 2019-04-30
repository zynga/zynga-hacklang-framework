<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\GDPR;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\Production as ReadProduction
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 * @extends ReadProduction
 */
class Production extends ReadProduction {

  const string DB_PASSWORD = 'JHBdo87MKR';

  <<__Override>>
  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    for ($i = 0; $i < self::PROD_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          sprintf('sexyHackGDPR%dR', $i),
          self::DB_PASSWORD,
          self::DB_HOSTNAME,
          self::PORT,
        ),
      );
    }

    return true;
  }

}
