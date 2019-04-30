<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\GDPR;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\Dev as ReadDev
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 * @extends ReadDev
 */
class Dev extends ReadDev {

  const string DB_PASSWORD = 'JHBdo87MKR';

  <<__Override>>
  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        'sexyHackGDPR0R',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    $this->addServer(
      new ConnectionDetails(
        'sexyHackGDPR1R',
        self::DB_PASSWORD,
        'poker-dev-adb-sexy-shard1.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com',
        self::PORT,
      ),
    );

    return true;
  }

}
