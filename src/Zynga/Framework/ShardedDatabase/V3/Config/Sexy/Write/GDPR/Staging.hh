<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\Staging as WriteStaging
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 */
class Staging extends WriteStaging {

  const string DB_PASSWORD = 'i6HKAEdzVR';

  <<__Override>>
  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        'sexyHackGDPR0W',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    $this->addServer(
      new ConnectionDetails(
        'sexyHackGDPR1W',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    return true;
  }

}
