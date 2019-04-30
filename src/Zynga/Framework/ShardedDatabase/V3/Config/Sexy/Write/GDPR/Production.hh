<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\Production as WriteProduction
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 */
class Production extends WriteProduction {

  const string DB_PASSWORD = 'i6HKAEdzVR';

  <<__Override>>
  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    for ($i = 0; $i < self::PROD_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          sprintf('sexyHackGDPR%dW', $i),
          self::DB_PASSWORD,
          self::DB_HOSTNAME,
          self::PORT,
        ),
      );
    }

    return true;
  }

}
