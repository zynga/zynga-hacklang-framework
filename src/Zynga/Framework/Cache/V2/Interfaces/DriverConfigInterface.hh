<?hh // strict

namespace Zynga\Framework\Cache\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverConfigInterface extends ConfigInterface {
  public function getServerPairings(): Map<string, int>;
  public function getDriver(): string;
  public function getStorableObjectName(): string;
  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string;
  public function getTTL(): int;

  // -----
  // Locking support
  // -----
  public function createLockKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string;

  // Although this supports returning any arbitrary type,
  // you should use type refinement in your implementation to specify a concrete type.
  public function createLockPayloadFromStorableObject(
    StorableObjectInterface $obj,
  ): mixed;

  public function getLockFlags(StorableObjectInterface $obj): int;

  /**
   * @return int This should return a relative duration from now rather than a timestamp.
   */
  public function getLockTTL(StorableObjectInterface $obj): int;
}
