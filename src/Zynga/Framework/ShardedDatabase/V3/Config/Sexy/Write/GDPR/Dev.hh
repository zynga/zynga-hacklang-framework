<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\GDPR;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write\Dev as WriteDev
;
use Zynga\Poker\Framework\Database\Users;

/**
 * Connection for gdpr usage
 */
class Dev extends WriteDev {

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
