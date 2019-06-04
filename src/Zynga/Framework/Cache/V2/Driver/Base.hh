<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Exceptions\CacheDoesNotSupportKeyOverride;
use Zynga\Framework\Cache\V2\Exceptions\CacheDoesNotSupportTTLOverride;
use Zynga\Framework\Cache\V2\Exceptions\CacheRequiresTTLException;
use Zynga\Framework\Cache\V2\Exceptions\CacheTTLExceededException;
use Zynga\Framework\Factory\V2\Driver\Base as FactoryDriverBase;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

abstract class Base extends FactoryDriverBase implements DriverInterface {

  public function getKeySupportingOverride(
    StorableObjectInterface $obj,
    string $keyOverride,
  ): string {

    $cacheAllowsKeyOverride = $this->getConfig()->cacheAllowsKeyOverride();
    if ($cacheAllowsKeyOverride === false && $keyOverride != '') {
      throw new CacheDoesNotSupportKeyOverride(
        'cache='.get_class($this->getConfig()).' key='.$keyOverride,
      );
    }

    $key = $keyOverride;

    if ($keyOverride == '') {
      $key = $this->getConfig()->createKeyFromStorableObject($obj);
    }

    return $key;

  }

  public function getTTLSupportingOverride(int $ttlOverride): int {

    if ($this->getConfig()->cacheAllowsTTLOverride() === false &&
        $ttlOverride != -1) {
      throw new CacheDoesNotSupportTTLOverride(
        'cache='.get_class($this->getConfig()).' key='.$ttlOverride,
      );
    }

    $ttl = $ttlOverride;
    $cacheTTL = $this->getConfig()->getTTL();

    // Check to see fit he TTL was not overriden.
    if ($ttl == -1) {
      $ttl = $cacheTTL;
    }

    if ($this->getConfig()->cacheAllowsNonExpiringKeys() === false &&
        $ttl === 0) {
      throw new CacheRequiresTTLException(
        'class='.
        get_class($this->getConfig()).
        ' requires a ttl to be provided.',
      );
    }

    if ($ttl > $cacheTTL) {
      throw new CacheTTLExceededException(
        'class='.
        get_class($this->getConfig()).
        ' cacheTTL='.
        $cacheTTL.
        ' overrideTTL='.
        $ttlOverride,
      );
    }

    return $ttl;

  }

}
