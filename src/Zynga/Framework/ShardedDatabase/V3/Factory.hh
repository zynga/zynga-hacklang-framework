<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Factory\V2\Base as FactoryBase;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as DriverIterator;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Iterable as DriverIterable;
use Zynga\Framework\ShardedDatabase\V3\Driver\Mock as UserShardedMockDatabaseDriver;
use Zynga\Framework\ShardedDatabase\V3\Info as ShardInfo;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;

class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return '\Zynga\Framework\ShardedDatabase\V3';
  }

  public static function getDriver<TType as TypeInterface>(
    string $configName,
    TType $shardType,
  ): DriverInterface<TType> {
    $dbh = self::factory(DriverInterface::class, $configName);
    $dbh->setShardType($shardType);
    return $dbh;
  }
  
  public static function getMockDriver(): DriverInterface<UInt64Box> {
    return self::getDriver('Mock', new UInt64Box(1));
  }
  
  public static function getIterator<TType as TypeInterface>(string $configName, TType $shardType): DriverIterable<TType> {
    $driver = self::getDriver($configName, $shardType);
    return new DriverIterator($driver->getConfig(), $shardType);
  }
  
  public static function getMockIterator(): DriverIterable<UInt64Box> {
    $driver = self::getMockDriver();
    return new DriverIterator($driver->getConfig(), new UInt64Box(1));
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
