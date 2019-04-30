<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read\GDPR;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Read\Staging as ReadStaging
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 * @extends ReadDev
 */
class Staging extends ReadStaging {

  <<__Override>>
  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    // for staging we simulate it by adding as many shards as prod.
    for ($i = 0; $i < self::STAGING_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          Users::GDPR_READ_USER,
          Users::GDPR_READ_PWD,
          'poker-stg-adb-fb-shard0.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com',
          self::PORT,
        ),
      );
    }

    return true;
  }

}
