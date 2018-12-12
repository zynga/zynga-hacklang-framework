<?hh // strict

namespace Zynga\Framework\Service\V2\Server;

use Zynga\Framework\Service\V2\Interfaces\ServerInterface;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;

abstract class Base extends StorableObject implements ServerInterface {}
