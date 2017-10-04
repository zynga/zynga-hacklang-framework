<?hh // strict

namespace Zynga\Framework\Datadog\V2;

use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Datadog\V2\Interfaces\DriverInterface;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Factory\V2\Base as FactoryBase;

/**
 * Factory for the creation and retrieval of Datadog drivers
 */
class Factory extends FactoryBase {

  /**
   * Retrieves this class's root path
   *
   * @return Root path for this class
   */
  public static function getClassRoot(): string {
    return '\Zynga\Framework\Datadog\V2';
  }

}
