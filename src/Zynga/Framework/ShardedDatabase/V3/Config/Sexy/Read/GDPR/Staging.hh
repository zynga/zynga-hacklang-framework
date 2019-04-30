<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\GDPR;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\Staging as ReadStaging
;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 * @extends ReadDev
 */
class Staging extends ReadStaging {

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
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    return true;
  }

}
