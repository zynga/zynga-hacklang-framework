<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Interfaces;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as BaseDriverInterface
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverInterface extends BaseDriverInterface {

  /**
   *
   * Fetches the configuration for this driver instance.
   *
   * @return DriverConfigInterface
   */
  public function getConfig(): DriverConfigInterface;
  
  public function isLocked(StorableObjectInterface $obj): bool;
  
  /**
   *
   * Locking a existing object if possible, exception if not capable.
   *
   * @param StorableObjectInterface $obj
   * @param bool lock success state
   *
   */
  public function lock(StorableObjectInterface $obj): bool;

  /**
   *
   * Given a existing object that is locked, unlock it.
   * If we didn't have a lock on the object, toss an exception about the issue.
   *
   * @param StorableObjectInterface $obj
   * @return bool unlock success state
   *
   */
  public function unlock(StorableObjectInterface $obj): bool;

  /**
   *
   * We assume you want the object locked as part of this api, so the lock is
   * established on fetch.
   *
   * @param StorableObjectInterface $obj
   * @return bool object return
   */
  public function get(
    StorableObjectInterface $obj,
    bool $getLocked = false,
  ): ?StorableObjectInterface;

  /**
   *
   * If the object isn't locked previously via a manual lock() moment,
   * you will get an exception on set.
   *
   * @param bool releaseLockOnSet releases the lock upon a successful set moment. defaults to always.
   * @return bool set success
   *
   */
  public function set(
    StorableObjectInterface $obj,
    bool $releaseLockOnSet = true,
  ): bool;

  /**
   *
   * If the object isn't locked previously via a manual lock() moment,
   * you will get an exeception on set.
   *
   * @param StorableObjectInterface $obj object you would like deleted.
   * @param bool $releaseLockOnDelete releases the lock upon a succcessful lock moment, defaults to always.
   * @return bool delete success.
   *
   */
  public function delete(
    StorableObjectInterface $obj,
    bool $releaseLockOnDelete = true,
  ): bool;

}
