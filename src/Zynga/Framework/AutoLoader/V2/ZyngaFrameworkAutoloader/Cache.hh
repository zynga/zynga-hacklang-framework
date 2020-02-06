<?hh // strict

namespace Zynga\Framework\AutoLoader\V2\ZyngaFrameworkAutoLoader;

use \Exception;
use \Memcached;

class Cache {
  private static ?Memcached $_mc;

  public static function makeCacheSafeKey(string $class): string {
    return 'z:fal:'.md5(__FILE__).':'.md5($class);
  }

  public static function getConnection(): Memcached {

    if (self::$_mc instanceof Memcached) {
      return self::$_mc;
    }

    $mc = new Memcached();
    $mc->addServer('localhost', 11211);

    return $mc;

  }

  public static function storeInCache(
    string $class,
    string $path,
    int $ttl = 86400,
  ): bool {
    $key = self::makeCacheSafeKey($class);
    $mc = self::getConnection();
    return $mc->add($key, $path, $ttl);
  }

  public static function getFromCache(string $class): ?string {

    $key = self::makeCacheSafeKey($class);

    $mc = self::getConnection();

    $value = $mc->get($key);

    if (is_string($value) === true) {
      return $value;
    }

    return null;
  }

}
