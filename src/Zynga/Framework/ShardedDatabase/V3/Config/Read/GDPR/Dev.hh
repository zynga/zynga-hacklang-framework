<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read\GDPR;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\ShardedDatabase\V3\Config\Read\Dev as ReadDev;
use Zynga\Poker\Framework\Database\Users;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * Connection for gdpr usage
 * @extends ReadDev
 */
class Dev extends ReadDev {

  <<__Override>>
  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        Users::GDPR_READ_USER,
        Users::GDPR_READ_PWD,
        'poker-dev-adb-fb-shard0.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com',
        self::PORT,
      ),
    );

    return true;
  }

}
