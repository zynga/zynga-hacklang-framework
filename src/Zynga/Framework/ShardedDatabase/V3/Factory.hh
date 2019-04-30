<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Factory\V2\Base as FactoryBase;
use Zynga\Legacy\SexyShards\V2\SexyShardsFactory;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as DriverIterator
;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Iterable as DriverIterable
;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\Mock as UserShardedMockDatabaseDriver
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\ShardedDatabase\V3\Info as ShardInfo;

class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return '\Zynga\Framework\ShardedDatabase\V3';
  }

  public static function getDriver(
    string $driverName,
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    $dbh = self::factory(DriverInterface::class, $driverName);
    $dbh->setSnUid($sn, $uid);
    $database = ShardInfo::getDatabaseSchemaForSocialNetworkId($sn);
    $dbh->getConfig()->setCurrentDatabase($database);
    return $dbh;
  }

  public static function getRead(SnidBox $sn, UidBox $uid): DriverInterface {
    return self::getDriver('Read', $sn, $uid);
  }

  public static function getWrite(SnidBox $sn, UidBox $uid): DriverInterface {
    return self::getDriver('Write', $sn, $uid);
  }

  public static function getReadWithoutUid(SnidBox $sn): DriverInterface {
    return self::getRead($sn, new UidBox(1));
  }

  public static function getReadWithoutSnUid(): DriverInterface {
    return self::getRead(SnidBoxFactory::facebook(), new UidBox(1));
  }

  public static function getReadIterator(SnidBox $sn): DriverIterable {
    $driver = self::getReadWithoutUid($sn);
    return new DriverIterator($driver->getConfig(), $sn);
  }

  public static function getGDPRRead(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Read_GDPR', $sn, $uid);
  }

  public static function getGDPRWrite(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Write_GDPR', $sn, $uid);
  }

  public static function getSexyRead(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Sexy_Read', $sn, $uid);
  }

  public static function getSexyWrite(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Sexy_Write', $sn, $uid);
  }

  public static function getSexyGDPRRead(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Sexy_Read_GDPR', $sn, $uid);
  }

  public static function getSexyGDPRWrite(
    SnidBox $sn,
    UidBox $uid,
  ): DriverInterface {
    return self::getDriver('Sexy_Write_GDPR', $sn, $uid);
  }

  public static function loadResultsForTest(
    string $class,
    string $function,
  ): bool {
    try {
      $dbh = self::factory(UserShardedMockDatabaseDriver::class, 'Mock');
      return $dbh->loadResultsForTest($class, $function.'_UserSharded');
    } catch (Exception $e) {
      throw $e;
    }
  }

}
