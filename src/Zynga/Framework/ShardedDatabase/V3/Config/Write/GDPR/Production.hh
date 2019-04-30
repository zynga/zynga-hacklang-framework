<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Write\GDPR;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Write\Production as WriteProduction
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 */
class Production extends WriteProduction {

  <<__Override>>
  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    for ($i = 0; $i < self::PROD_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          Users::GDPR_WRITE_USER,
          Users::GDPR_WRITE_PWD,
          sprintf(
            "poker-adb-fb-shard%d.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com",
            $i,
          ),
          self::PORT,
        ),
      );
    }

    return true;
  }

}
