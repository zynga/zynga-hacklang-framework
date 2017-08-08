<?hh // strict

namespace Zynga\Framework\Cache\V2\Config;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;

abstract class Base extends FactoryBaseConfig
  implements DriverConfigInterface {}
