<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Config;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;
use Zynga\Framework\Lockable\Cache\V1\LockPayload;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

abstract class Base implements DriverConfigInterface {

  /**
   *
   * Most implementations use the default LockPayload, but you can overload it
   * if needed.
   *
   * @return LockPayLoadInterface
   *
   */
  public function getPayloadObject(): LockPayloadInterface {
    $payload = new LockPayload();
    $payload->setLockEstablishment(time());
    // --
    // TODO: add in the backtrace from the caller so we can debug naughty locks.
    // --
    $payload->setBacktrace('TODO');
    return $payload;
  }

}
