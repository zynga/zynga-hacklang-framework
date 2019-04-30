<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Write\GDPR;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Write\Staging as WriteStaging
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 */
class Staging extends WriteStaging {

  <<__Override>>
  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    // for staging we simulate it by adding as many shards as prod.
    for ($i = 0; $i < self::STAGING_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          Users::GDPR_WRITE_USER,
          Users::GDPR_WRITE_PWD,
          'poker-stg-adb-fb-shard0.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com',
          self::PORT,
        ),
      );
    }

    return true;

  }
}
