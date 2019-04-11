<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use
  Zynga\Framework\Cache\V2\Exceptions\NotSupportedException as NotSupportedException
;

class LockingMemcacheTest extends BaseMemcacheTest {
  public function getDriverName(): string {
    return 'Mock_LockingCache';
  }

  public function getNonstorableDriverName(): string {
    return 'Mock_LockingCache_NonStorableObject';
  }

  public function getNoServersConfiguredDriverName(): string {
    return 'Mock_LockingCache_NoServersConfigured';
  }

  public function testCanSetAndGetStorable(): void {
    $this->lockStorable();
    parent::testCanSetAndGetStorable();
    $this->unlockStorable();
  }

  public function testCanGetByMap(): void {
    $this->lockStorable();
    parent::testCanGetByMap();
    $this->unlockStorable();
  }

  public function testCanDeleteByStorable(): void {
    $this->lockStorable();
    parent::testCanDeleteByStorable();
    $this->unlockStorable();
  }

  public function testCanDeleteByMap(): void {
    $this->lockStorable();
    parent::testCanDeleteByMap();
    $this->unlockStorable();
  }

  protected function lockStorable(): void {
    $obj = $this->getTestModel();
    $cache =
      CacheFactory::factory(LockingMemcache::class, $this->getDriverName());

    $cache->lock($obj);
  }

  protected function unlockStorable(): void {
    $obj = $this->getTestModel();
    $cache =
      CacheFactory::factory(LockingMemcache::class, $this->getDriverName());
    $cache->unlock($obj);
  }
}
